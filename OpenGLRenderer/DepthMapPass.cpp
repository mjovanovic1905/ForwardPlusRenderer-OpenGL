#include "DepthMapPass.h"

#include <GL/glew.h>

#include "ShadowMaps.h"
#include "Window.h"

DepthMapPass::DepthMapPass(
    const std::function<void(ShaderProgram&)>& Draw,
    const ShaderProgram& shader,
    const ShadowMaps& shadowMaps,
    unsigned int resolution)
: RenderPass(Draw, shader)
, shadowResolution_(resolution)
{
    framebuffer_.Init();
    framebuffer_.AttachTexture(GL_DEPTH_ATTACHMENT, shadowMaps.GetShadowMapTextureId(), GL_TEXTURE_3D);
    assert(framebuffer_.IsComplete());
    Framebuffer::BindDefault();
}


void DepthMapPass::PreDraw()
{
    framebuffer_.Bind();
    glViewport(0, 0, shadowResolution_, shadowResolution_);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);

    shader_.UseProgram();
    shader_.SetUniformBuffer("LightSpaceMatrices", 0);
}

void DepthMapPass::PostDraw()
{
    const Window& window = Window::Get();
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glCullFace(GL_BACK);
    Framebuffer::BindDefault();
}

DepthMapPass::~DepthMapPass()
{
}

