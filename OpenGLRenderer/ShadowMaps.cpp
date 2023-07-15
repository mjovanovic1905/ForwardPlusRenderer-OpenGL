#include "ShadowMaps.h"

#include <glm/gtc/matrix_transform.hpp>

std::vector<glm::vec4> ShadowMaps::GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

glm::mat4 ShadowMaps::GetLightSpaceMatrix(
    const float nearPlane,
    const float farPlane,
    const Camera& camera,
    const glm::vec3& lightDir)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width = viewport[2];
    float height = viewport[3];
    
    const auto proj = glm::perspective(
        glm::radians(camera.GetFOV()), width / height, nearPlane,
        farPlane);
    const auto corners = GetFrustumCornersWorldSpace(proj, camera.GetViewMatrix());

    glm::vec3 center = glm::vec3(0, 0, 0);
    for (const auto& v : corners)
    {
        center += glm::vec3(v);
    }
    center /= corners.size();

    const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }
    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return lightProjection * lightView;
}

std::vector<glm::mat4> ShadowMaps::GetLightSpaceMatrices(
    const std::vector<float>& shadowCascadeLevels,
    const Camera& camera,
    const glm::vec3& lightDir)
{
    std::vector<glm::mat4> ret;
    for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(GetLightSpaceMatrix(Camera::NEAR_PLANE, shadowCascadeLevels[i], camera, lightDir));
        }
        else if (i < shadowCascadeLevels.size())
        {
            ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i], camera, lightDir));
        }
        else
        {
            ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], Camera::FAR_PLANE, camera, lightDir));
        }
    }

    return ret;
}

ShadowMaps::ShadowMaps(
    const std::vector<float>& shadowCascadeLevels,
    const Camera& camera,
    const DirectionalLight& light,
    int shadowResolution,
    int depthMapTextureUnit
)
: camera_(camera)
, light_(light)
, shadowCascadeLevels_(shadowCascadeLevels)
{
    depthMapTextures_.Init(shadowResolution, depthMapTextureUnit, shadowCascadeLevels_.size() + 1);
    lightTransformationMatricesGPU_.Init(nullptr, shadowCascadeLevels_.size() + 1);
}

void ShadowMaps::GenerateShadows()
{
    const auto lightMatrices = GetLightSpaceMatrices(shadowCascadeLevels_, camera_, light_.direction);
    lightTransformationMatricesGPU_.Bind();
    
    for (size_t i = 0; i < lightMatrices.size(); ++i)
    {
        lightTransformationMatricesGPU_.InsertData((glm::mat4*)&lightMatrices[i], i * sizeof(glm::mat4));
    }

    UniformBuffer<void>::UnbindBuffer();
}

void ShadowMaps::BindShadowMapTexture()
{
    depthMapTextures_.Bind();
}

ShadowMaps::~ShadowMaps()
{
}
