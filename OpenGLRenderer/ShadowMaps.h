#ifndef _SHADOW_MAPS_
#define _SHADOW_MAPS_

#include <vector>

#include <glm/glm.hpp>

#include "UniformBuffer.h"
#include "TextureArray.h"
#include "Camera.h"
#include "DirectionalLight.h"


//Abstraction for cascaded shadow mapping

class ShadowMaps
{
public:
    ShadowMaps(
        const std::vector<float>& shadowCascadeLevels,
        const Camera& camera,
        const DirectionalLight& light,
        int shadowResolution,
        int depthMapTextureUnit
    );

    void GenerateShadows();
    void BindShadowMapTexture();
    unsigned int GetLightTransformationId() const { return lightTransformationMatricesGPU_.GetId(); }
    unsigned int GetShadowMapTextureId() const { return depthMapTextures_.GetId(); }

    ~ShadowMaps();

private:
    static std::vector<glm::mat4> GetLightSpaceMatrices(
        const std::vector<float>& shadowCascadeLevels,
        const Camera& camera,
        const glm::vec3& lightDir);

    static glm::mat4 GetLightSpaceMatrix(
        const float nearPlane,
        const float farPlane,
        const Camera& camera,
        const glm::vec3& lightDir);
    
    static std::vector<glm::vec4> GetFrustumCornersWorldSpace(
        const glm::mat4& proj,
        const glm::mat4& view);

    const Camera camera_;
    const DirectionalLight light_;

    std::vector<float> shadowCascadeLevels_;
    UniformBuffer<glm::mat4> lightTransformationMatricesGPU_;
    TextureArray depthMapTextures_;
};

#endif