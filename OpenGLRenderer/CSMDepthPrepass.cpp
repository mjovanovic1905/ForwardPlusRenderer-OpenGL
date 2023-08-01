#include "CSMDepthPrepass.h"

#include <GL/glew.h>

#include "CSMShadowMaps.h"
#include "Window.h"

CSMDepthPrepass::CSMDepthPrepass(
    const std::function<void(ShaderProgram&)>& Draw, ShaderProgram shader,
    CSMShadowMaps& shadowMaps,
    unsigned int resolution)
: RenderPass(Draw, shader)
, shadowResolution_(resolution)
, shadowMaps_(shadowMaps)
{
    framebuffer_.Init();
    framebuffer_.AttachTexture(GL_DEPTH_ATTACHMENT, shadowMaps_.GetShadowMapTextureId(), GL_TEXTURE_3D);
    assert(framebuffer_.IsComplete());
    Framebuffer::BindDefault();
}


void CSMDepthPrepass::PreDraw()
{
    framebuffer_.Bind();
    glViewport(0, 0, shadowResolution_, shadowResolution_);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowMaps_.GenerateShadows();
    shadowMaps_.BindShadowMapTexture();

    shader_.UseProgram();
    shader_.SetUniformBuffer("LightSpaceMatrices", 0);
}

void CSMDepthPrepass::PostDraw()
{
    const Window& window = Window::Get();
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glCullFace(GL_BACK);
    Framebuffer::BindDefault();
}

CSMDepthPrepass::~CSMDepthPrepass()
{
}

