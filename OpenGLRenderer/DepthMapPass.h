#pragma once

#include <functional>

#include "RenderPass.h"
#include "Framebuffer.h"

class CSMShadowMaps;

class DepthMapPass : public RenderPass
{
public:
    DepthMapPass(
        const std::function<void(ShaderProgram&)>& Draw,
        const ShaderProgram& shader,
        CSMShadowMaps& shadowMaps,
        unsigned int resolution);

    virtual ~DepthMapPass() override;

    virtual void PreDraw() override;
    virtual void PostDraw() override;

private:
    Framebuffer framebuffer_;
    unsigned int shadowResolution_;
    CSMShadowMaps& shadowMaps_;
};