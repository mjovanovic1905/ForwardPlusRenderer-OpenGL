#ifndef DEBUG_LIGHT_PASS
#define DEBUG_LIGHT_PASS

#include <vector>

#include "DrawableObject.h"
#include "PointLight.h"
#include "ShaderProgram.h"

class DrawDebugLights : public DrawableObject
{
public:
    DrawDebugLights(const std::vector<PointLight>& pointLights);
    ~DrawDebugLights();

    void Draw(ShaderProgram& shader) override;

private:
    
    struct DebugLightVertex
    {
        DebugLightVertex(const glm::vec3& p) : position(p) {}
        glm::vec3 position;
    };
    
    void SetupDebugCubes(
        const std::vector<PointLight>& pointLights,
        std::vector<DebugLightVertex>& vertices,
        std::vector<unsigned int>& indices);
    
    void SetupVertices(const PointLight& pointLights, std::vector<DebugLightVertex>& vertices);
    void SetupIndices(std::vector<unsigned int>& indices, int i);
};

#endif