#ifndef _DEPTH_MAP_PASS_
#define _DEPTH_MAP_PASS_

#include <functional>

#include "RenderPass.h"
#include "Framebuffer.h"

class ShadowMaps;

class DepthMapPass : public RenderPass
{
public:
    DepthMapPass(
        const std::function<void(ShaderProgram&)>& Draw,
        const ShaderProgram& shader,
        const ShadowMaps& shadowMaps,
        unsigned int resolution);

    virtual ~DepthMapPass() override;

    virtual void PreDraw() override;
    virtual void PostDraw() override;

private:
    Framebuffer framebuffer_;
    unsigned int shadowResolution_;

};

#endif