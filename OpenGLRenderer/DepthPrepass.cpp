#include "DepthPrepass.h"

#include <GL/glew.h>

#include "Defines.h"
#include "Camera.h"
#include "Window.h"

DepthPrepass::DepthPrepass(const std::function<void(ShaderProgram&)>& Draw, const ShaderProgram& shader)
: RenderPass(Draw, shader)
{
    const Window& window = Window::Get();
    depthTexture_.InitForWrite(window.GetWidth(), window.GetHeight(), GL_DEPTH_COMPONENT, 2);
    depthTexture_.BindTexture();
    framebuffer_.Init(true);
    framebuffer_.Bind();
    framebuffer_.AttachTexture(GL_DEPTH_ATTACHMENT, depthTexture_.GetId(), GL_TEXTURE_2D);
    Framebuffer::BindDefault();
    assert(framebuffer_.IsComplete());
}

void DepthPrepass::PreDraw()
{
    const Window& window = Window::Get();   
    framebuffer_.Bind();
    depthTexture_.BindTexture();
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    glClear(GL_DEPTH_BUFFER_BIT);

    shader_.UseProgram();
}

void DepthPrepass::PostDraw()
{
    Framebuffer::BindDefault();
}

DepthPrepass::~DepthPrepass()
{
}

