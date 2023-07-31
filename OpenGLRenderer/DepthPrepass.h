#pragma once

#include "RenderPass.h"
#include "Framebuffer.h"
#include "Texture.h"

class DepthPrepass : public RenderPass
{
public:
    DepthPrepass(const std::function<void(ShaderProgram&)>& Draw, const ShaderProgram& shader);
    virtual ~DepthPrepass() override;

    Texture& GetDepthMap() { return depthTexture_; }

    virtual void PreDraw() override;
    virtual void PostDraw() override;

private:

    Framebuffer framebuffer_;
    Texture depthTexture_;

};