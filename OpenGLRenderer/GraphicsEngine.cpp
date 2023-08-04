#include "GraphicsEngine.h"
#include "EngineUtils.h"
#include "Window.h"
#include <GLFW/glfw3.h>

GraphicsEngine::GraphicsEngine()
    : camera_(glm::vec3(0.0f, 100.0f, -24.0f))
    , graphicsUtils_(camera_)
    , drawPass_(graphicsUtils_.SetupMainPass())
    , csmDepthPrepass_(graphicsUtils_.SetupCSMDepthPrepass())
    , depthPrepass_(graphicsUtils_.SetupLightCullingDepthPrepass())
    , computeShader_(graphicsUtils_.SetupLightCullingComputeShader(depthPrepass_))
    , debugLights_(graphicsUtils_.SetupDrawDebugLights())
    , drawDepthMap_(graphicsUtils_.SetupLightCullingDepthMapDrawPass(depthPrepass_))
{
}

void GraphicsEngine::Run()
{
    Window& mainWindow = Window::Get();

    EngineUtils::SetupOpenGl();

    float lastFrame = 0.0f;

    while (!mainWindow.WindowShouldClose())
    {
        mainWindow.HandleResizeEvent();

        float currentFrame = glfwGetTime();
        camera_.ProcessInput(currentFrame - lastFrame);
        lastFrame = currentFrame;

        csmDepthPrepass_.Draw();

        if constexpr (EngineUtils::USE_LIGHT_CULLING)
        {
            PerformLightCulling();
        }

        if constexpr (EngineUtils::DRAW_LIGHT_CULLING_DEPTH_MAP)
        {
            drawDepthMap_.Draw();
        }

        else
        {
            drawPass_.Draw();

            if constexpr (EngineUtils::DRAW_DEBUG_LIGHTS)
            {
                debugLights_.Draw();
            }
        }


        mainWindow.SwapBuffers();
        glfwPollEvents();

        EngineUtils::PrintFrameTime();
    }

    glfwTerminate();
}

void GraphicsEngine::PerformLightCulling()
{
    graphicsUtils_.GetPointLightBuffer().Bind();
    depthPrepass_.Draw();

    computeShader_.UseProgram();
    computeShader_.SetUniformValue("invViewProj", glm::inverse(camera_.GetProjectionMatrix() * camera_.GetViewMatrix()));
    depthPrepass_.GetDepthMap().BindTexture();
    computeShader_.Execute();
    computeShader_.Wait();
}
