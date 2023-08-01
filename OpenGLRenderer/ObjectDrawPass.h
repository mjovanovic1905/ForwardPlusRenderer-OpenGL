#pragma once

#include <optional>
#include <memory>

#include "RenderPass.h"
#include "ShaderProgram.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "Renderbuffer.h"
#include "FXAA.h"

class Camera;
class DirectionalLight;
class Window;

class ObjectDrawPass : public RenderPass
{
public:
    ObjectDrawPass(
        const std::function<void(ShaderProgram&)>& Draw, ShaderProgram shader,
        const Camera& camera,
        const DirectionalLight& light,
        const std::vector<PointLight>& pointLights
    );

    virtual ~ObjectDrawPass() override;

    virtual void PreDraw() override;
    virtual void PostDraw() override;

    void UseFXAA();


private: 

    void SetupCustomFramebuffer();
    inline bool isUsingFXAA() const;
    
    const Camera& camera_;
    const DirectionalLight& light_;
    const std::vector<PointLight>& pointLights_;
    
    std::optional<Framebuffer> framebuffer_;
    std::shared_ptr<Texture> targetTexture_;
    std::shared_ptr<Texture> depthTexture_;
    std::optional<Renderbuffer> depthBuffer_;

    std::optional<FXAA> fxaa_;
};