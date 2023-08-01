#include "DrawLightCullingDepthMap.h"

#include "EngineUtils.h"

DrawLightCullingDepthMap::DrawLightCullingDepthMap(Texture& deptMap)
    : deptMap_(deptMap)
{
    struct ShadowDBGVertex
    {
        ShadowDBGVertex(glm::vec3 p, glm::vec2 t) : position(p), texCoord(t) {}
        glm::vec3 position;
        glm::vec2 texCoord;
    };
    std::vector<ShadowDBGVertex> quadVertices;
    quadVertices.push_back(ShadowDBGVertex(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
    quadVertices.push_back(ShadowDBGVertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
    quadVertices.push_back(ShadowDBGVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
    quadVertices.push_back(ShadowDBGVertex(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));

    vao_.Init();
    vao_.Bind();
    std::vector<VertexAttributeDescription> attribDescriptions;
    attribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::POSITION));
    attribDescriptions.push_back(VertexAttributeDescription(2, false, VertexAttributeType::TEX_COORDS));
    vbo_.Init(quadVertices, attribDescriptions);
    vbo_.Bind();

    ShaderData vertexShader;
    vertexShader.sourceCode = EngineUtils::ReadFile("./Shaders/depthDBG.vert");
    ShaderData fragmentShader;
    fragmentShader.sourceCode = EngineUtils::ReadFile("./Shaders/depthDBG.frag");

    shaderProgram_.Init(&vertexShader, &fragmentShader);
    shaderProgram_.UseProgram();
}

void DrawLightCullingDepthMap::Draw(ShaderProgram& shader)
{

}
