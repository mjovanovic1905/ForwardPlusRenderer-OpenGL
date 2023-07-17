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

#include "Defines.h"
#include "Window.h"
#include "Utils.h"
#include "VAO.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "MaterialData.h"
#include "LightProperties.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Model.h"
#include "Renderbuffer.h"
#include "Framebuffer.h"
#include "TextureArray.h"
#include "StorageBuffer.h"
#include "ShadowMaps.h"
#include "DepthMapPass.h"
#include "ObjectDrawPass.h"
#include "Cubemap.h"
#include "DrawDebugLights.h"
#include "LightGenerator.h"
#include "DepthPrepass.h"
#include "PointLightBuffer.h"
#include "ComputeShader.h"

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
        vertexShader.sourceCode = ReadFile("./Shaders/depthDBG.vert");
        ShaderData fragmentShader;
        fragmentShader.sourceCode = ReadFile("./Shaders/depthDBG.frag");

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

void printFrameTime()
{
    static double lastTime = glfwGetTime();
    static unsigned int numOfFrames = 0;

    double currentTime = glfwGetTime();
    numOfFrames++;
    if (currentTime - lastTime > 1.f)
    {
        printf("%f ms/frame\n", 1000.0/double(numOfFrames));
        numOfFrames = 0;
        lastTime += 1.0;
    }   
}

int main()
{

    
    if (glewIsExtensionSupported("GL_ARB_arrays_of_arrays")) {
        bool nestoDa = true;
    }
    
    if (!InitLibraries())
    {
        ReleaseLibraryData();
        return -1;
    }


    Window& mainWindow = Window::Get();
    
    Camera camera(mainWindow);
    camera.Init(glm::vec3(0.0f, 100.0f, -24.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
    if constexpr (MSAA_ENABLED)
    {
        glEnable(GL_MULTISAMPLE);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    std::vector<float> cubeVertices = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    const std::vector<float> planeVertices = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    std::vector<float> quadVertices = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    std::vector<float> shadowCascadeLevels
    {
        Camera::FAR_PLANE / 50.0f,
        Camera::FAR_PLANE / 25.0f,
        Camera::FAR_PLANE / 10.0f,
        Camera::FAR_PLANE / 2.0f
    };

    std::vector<float> skyboxVertices = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    int numCSMPlanes = shadowCascadeLevels.size() + 1;

    ShaderData vertexShaderData;
    vertexShaderData.sourceCode = ReadFile("./Shaders/blinn-phong.vert");
    ShaderData fragmentShaderData;
    fragmentShaderData.sourceCode = ReadFile("./Shaders/blinn-phong-fp.frag");
    fragmentShaderData.defines.push_back(ShaderDefine("NUM_CSM_PLANES", std::to_string(numCSMPlanes)));
    ShaderProgram shaderProgram;
    if (!shaderProgram.Init(&vertexShaderData, &fragmentShaderData))
    {
        ReleaseLibraryData();
        return -1;
    }
    int workGroupsX = (WINDOW_WIDTH + (WINDOW_WIDTH % 16)) / 16;
    int workGroupsY = (WINDOW_HEIGHT + (WINDOW_HEIGHT % 16)) / 16;
    shaderProgram.UseProgram();
    shaderProgram.SetUniformValue("farPlane", Camera::FAR_PLANE);
    shaderProgram.SetUniformValue("numOfTiles", workGroupsY);
    shaderProgram.SetUniformValue("numLights", 17);
    for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
    {
        shaderProgram.SetUniformValue(("cascadePlaneDistances[" + std::to_string(i) + "]").c_str(), shadowCascadeLevels[i]);
    }

    ShaderData depthMapVertexShader;
    depthMapVertexShader.sourceCode = ReadFile("./Shaders/layeredDepthMap.vert"); 
    ShaderData depthMapFragmentShader;
    depthMapFragmentShader.sourceCode = ReadFile("./Shaders/layeredDepthMap.frag"); 
    ShaderData depthMapGeometryShader;
    depthMapGeometryShader.sourceCode = ReadFile("./Shaders/layeredDepthMap.geom"); 
    depthMapGeometryShader.defines.push_back(ShaderDefine("NUM_CSM_PLANES", std::to_string(numCSMPlanes)));
    ShaderProgram depthMapShader;
    if (!depthMapShader.Init(&depthMapVertexShader, &depthMapFragmentShader, &depthMapGeometryShader))
    {
        ReleaseLibraryData();
        return -1;
    }

    
    DirectionalLight light;
    light.direction = glm::normalize(glm::vec3(20.0f, 50, 20.0f));
    light.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    light.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    light.specular = glm::vec3(0.5f, 0.5f, 0.5f);

    LightGenerator lightGenerator("../sponza/lights.txt");

    std::vector<VertexAttributeDescription> attribDescriptions;
    attribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::POSITION));
    attribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::NORMALS));
    attribDescriptions.push_back(VertexAttributeDescription(2, false, VertexAttributeType::TEX_COORDS));

    ShadowMaps shadowMaps(shadowCascadeLevels, camera, light, SHADOW_RES, 2);

    ShaderData skyboxVSData;
    skyboxVSData.sourceCode = ReadFile("./Shaders/cubemap_test.vert");
    ShaderData skyboxFSData;
    skyboxFSData.sourceCode = ReadFile("./Shaders/cubemap_test.frag");
    ShaderProgram skyboxShader;
    skyboxShader.Init(&skyboxVSData, &skyboxFSData);

    std::vector<VertexAttributeDescription> skyboxAttribDescriptions;
    skyboxAttribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::POSITION));

    Cubemap skybox;
    skybox.Init(skyboxVertices, skyboxAttribDescriptions, "../textures/skybox");

    Model mmodel("../sponza/", "sponza.obj", false);
    glm::mat4 model = glm::mat4(1.f);

    
    auto drawFunc = [&](ShaderProgram& shader)
    {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = camera.GetProjectionMatrix();

        mmodel.SetView(view);
        mmodel.SetProj(proj);
        mmodel.SetModel(model);
        mmodel.Draw(shader);

        // cubeMesh.setModel(glm::scale(glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0)), glm::vec3(0.5f)));
        // cubeMesh.setView(view);
        // cubeMesh.setProj(proj);
        // cubeMesh.Draw(shader);

        // cubeMesh.setModel(glm::scale(glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0)), glm::vec3(0.5f)));
        // cubeMesh.Draw(shader);

        // cubeMesh.setModel(glm::scale(glm::rotate(glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0)), glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))), glm::vec3(0.25)));
        // cubeMesh.Draw(shader);

        // glm::mat4 skyboxView = glm::mat4(glm::mat3(view));  
        // skybox.setProj(proj);
        // skybox.setView(skyboxView);
        // skybox.Draw(skyboxShader);
    };

    DepthMapPass depthMapPass(drawFunc, depthMapShader, shadowMaps, SHADOW_RES);
    ObjectDrawPass drawPass(drawFunc, shaderProgram, camera, light, lightGenerator.GetLights());
    
    if constexpr (!MSAA_ENABLED)
    drawPass.UseFXAA();
    
    float lastFrame = 0.0f;

    DrawDebugLights debugLights(lightGenerator.GetLights());
    
    ShaderData debugLightsVertexShaderData;
    debugLightsVertexShaderData.sourceCode = ReadFile("./Shaders/light_debug.vert");

    ShaderData debugLightsfragmentShaderData;
    debugLightsfragmentShaderData.sourceCode = ReadFile("./Shaders/light_debug.frag");
    
    ShaderProgram debugLightsShader;
    debugLightsShader.Init(&debugLightsVertexShaderData, &debugLightsfragmentShaderData);

    ShaderData depthPrepassVertexShader;
    depthPrepassVertexShader.sourceCode = ReadFile("./Shaders/depthMap.vert");

    ShaderData depthPrepassFragmentShader;
    depthPrepassFragmentShader.sourceCode = ReadFile("./Shaders/depthMap.frag");

    ShaderProgram depthPrepassShader;
    depthPrepassShader.Init(&depthPrepassVertexShader, &depthPrepassFragmentShader);
    DepthPrepass depthPrepass(drawFunc, depthPrepassShader);

    PointLightBuffer pointLightBuffer(lightGenerator.GetLights(),workGroupsX * workGroupsY , 1024);


    ShaderData computeShaderData;
    computeShaderData.sourceCode = ReadFile("./Shaders/lightCulling.comp");

    ComputeShader computeShader(computeShaderData, workGroupsX, workGroupsY);
    computeShader.UseProgram();
    computeShader.SetUniformValue("view", camera.GetViewMatrix());
    computeShader.SetUniformValue("viewProj", camera.GetProjectionMatrix() * camera.GetViewMatrix());
    computeShader.SetUniformValue("numLights", (int)lightGenerator.GetLights().size());
    computeShader.SetUniformValue("depthMap", (int)depthPrepass.GetDepthMap().GetTextureUnit());
    computeShader.SetUniformValue("nearPlane", Camera::NEAR_PLANE);
    computeShader.SetUniformValue("farPlane", Camera::FAR_PLANE);
    computeShader.SetStorageBuffer("LightBuffer", 1);
    computeShader.SetStorageBuffer("VisibleLightIndicesBuffer", 2);

    while(!mainWindow.WindowShouldClose())
    {
        mainWindow.HandleResizeEvent();

        float currentFrame = glfwGetTime();
        camera.ProcessInput(currentFrame - lastFrame);
        lastFrame = currentFrame;

        shadowMaps.GenerateShadows();
        shadowMaps.BindShadowMapTexture();

        depthMapPass.Draw();
        //drawPass.Draw();

        //debugLights.SetModel(model);
        //debugLights.SetView(camera.GetViewMatrix());
        //debugLights.SetProj(camera.GetProjectionMatrix());
        //debugLights.Draw(debugLightsShader);

        //depthPrepass.
        depthPrepass.Draw();

         //renderDepthMap(depthPrepass.GetDepthMap());

        computeShader.UseProgram();
        computeShader.SetUniformValue("view", camera.GetViewMatrix());
        computeShader.SetUniformValue("viewProj", camera.GetProjectionMatrix() * camera.GetViewMatrix());
        computeShader.SetUniformValue("depthMap", (int)depthPrepass.GetDepthMap().GetTextureUnit());
        depthPrepass.GetDepthMap().BindTexture();
         computeShader.Execute();
         computeShader.Wait();
         drawPass.Draw();

        // const auto proj = glm::perspective(
        // glm::radians(camera.getFOV()), (float)WINDOW_WIDTH / WINDOW_HEIGHT, Camera::NEAR_PLANE,
        // Camera::FAR_PLANE);
        // const auto inv = glm::inverse(proj * camera.getViewMatrix());
        // auto pt = inv * glm::vec4(camera.getPosition(), 1.0f);
        // pt /= pt.w;
        
        // printf("%f %f %f \n", pt[0], pt[1], pt[2]);

        mainWindow.SwapBuffers();
        glfwPollEvents();

        // std::cout << "Errors in frame:\n";
        // while (auto error = glGetError())
        // {
        //     assert(error == GL_NO_ERROR);
        // }

        printFrameTime();
    }  

    glfwTerminate();
    return 0;
}