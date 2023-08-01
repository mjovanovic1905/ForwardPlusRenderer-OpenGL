#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <assert.h>
#include <functional>
#include <set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "EngineUtils.h"
#include "VAO.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "MaterialData.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Model.h"
#include "Renderbuffer.h"
#include "Framebuffer.h"
#include "TextureArray.h"
#include "StorageBuffer.h"
#include "CSMShadowMaps.h"
#include "CSMDepthPrepass.h"
#include "ObjectDrawPass.h"
#include "Cubemap.h"
#include "DebugLightsPass.h"
#include "LightGenerator.h"
#include "LightCullingDepthPrepass.h"
#include "PointLightBuffer.h"
#include "ComputeShader.h"

#include "GraphicsUtils.h"

void renderDepthMap(Texture& deptMap)
{
    static VAO vao;
    static VertexBuffer vertexBuffer;
    static ShaderProgram shaderProgram;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!vao.IsInitialised())
    {
        struct ShadowDBGVertex
        {
            ShadowDBGVertex(glm::vec3 p, glm::vec2 t) : position(p), texCoord(t) {}
            glm::vec3 position;
            glm::vec2 texCoord;
        };
        std::vector<ShadowDBGVertex> quadVertices;
        quadVertices.push_back(ShadowDBGVertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
        quadVertices.push_back(ShadowDBGVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        quadVertices.push_back(ShadowDBGVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        quadVertices.push_back(ShadowDBGVertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));

        vao.Init();
        vao.Bind();
        std::vector<VertexAttributeDescription> attribDescriptions;
        attribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::POSITION));
        attribDescriptions.push_back(VertexAttributeDescription(2, false, VertexAttributeType::TEX_COORDS));
        vertexBuffer.Init(quadVertices, attribDescriptions);
        vertexBuffer.Bind();

        ShaderData vertexShader;
        vertexShader.sourceCode = EngineUtils::ReadFile("./Shaders/depthDBG.vert");
        ShaderData fragmentShader;
        fragmentShader.sourceCode = EngineUtils::ReadFile("./Shaders/depthDBG.frag");

        shaderProgram.Init(&vertexShader, &fragmentShader);
        shaderProgram.UseProgram();
    }

    const Window& window = Window::Get();
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    shaderProgram.UseProgram();
    shaderProgram.SetUniformValue("nearPlane", Camera::NEAR_PLANE);
    shaderProgram.SetUniformValue("farPlane", Camera::FAR_PLANE);
    vao.Bind();
    shaderProgram.SetUniformValue("depthMap", (int)deptMap.GetTextureUnit());
    deptMap.BindTexture();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

int main()
{
    if (!EngineUtils::InitGLFW())
    {
        EngineUtils::ReleaseGLFW();
        return -1;
    }
    Window& mainWindow = Window::Get();

    EngineUtils::SetupOpenGl();

    Camera camera;
    camera.Init(glm::vec3(0.0f, 100.0f, -24.0f));

    float lastFrame = 0.0f;

    


    GraphicsUtils graphicsUtils(camera);

    ObjectDrawPass drawPass = graphicsUtils.SetupMainPass();
    CSMDepthPrepass csmDepthPrepass = graphicsUtils.SetupCSMDepthPrepass();
    LightCullingDepthPrepass depthPrepass = graphicsUtils.SetupLightCullingDepthPrepass();
    ComputeShader computeShader = graphicsUtils.SetupLightCullingComputeShader(depthPrepass);
    DebugLightsPass debugLights = graphicsUtils.SetupDrawDebugLights();

    while(!mainWindow.WindowShouldClose())
    {
        mainWindow.HandleResizeEvent();

        float currentFrame = glfwGetTime();
        camera.ProcessInput(currentFrame - lastFrame);
        lastFrame = currentFrame;

        csmDepthPrepass.Draw();
        
        if constexpr (EngineUtils::USE_LIGHT_CULLING)
        {
            graphicsUtils.GetPointLightBuffer().Bind();
            depthPrepass.Draw();

            //renderDepthMap(depthPrepass.GetDepthMap());


            computeShader.UseProgram();
            computeShader.SetUniformValue("invViewProj", glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix()));
            depthPrepass.GetDepthMap().BindTexture();
            computeShader.Execute();
            computeShader.Wait();
        }
        
        drawPass.Draw();

        if constexpr (EngineUtils::DRAW_DEBUG_LIGHTS)
        {
            debugLights.Draw();
        }

        mainWindow.SwapBuffers();
        glfwPollEvents();

        // std::cout << "Errors in frame:\n";
        // while (auto error = glGetError())
        // {
        //     assert(error == GL_NO_ERROR);
        // }

        EngineUtils::PrintFrameTime();
    }  

    glfwTerminate();
    return 0;
}