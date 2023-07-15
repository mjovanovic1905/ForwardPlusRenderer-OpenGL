#include "RenderPass.h"

RenderPass::RenderPass(const std::function<void(ShaderProgram&)>& Draw, const ShaderProgram& shader)
: drawFunc_(Draw)
, shader_(shader)
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::Draw()
{
    PreDraw();
    drawFunc_(shader_);
    PostDraw();
}

