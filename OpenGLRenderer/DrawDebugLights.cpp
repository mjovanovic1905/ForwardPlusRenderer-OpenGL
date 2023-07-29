#include "DrawDebugLights.h"

#include <glm/glm.hpp>

#include "EngineUtils.h"


DrawDebugLights::DrawDebugLights(const std::vector<PointLight>& pointLights)
{
    std::vector<DebugLightVertex> vertices;
    std::vector<unsigned int> indices;
    SetupDebugCubes(pointLights, vertices, indices);
    std::vector<VertexAttributeDescription> attribDescriptions;
    attribDescriptions.push_back(VertexAttributeDescription(3, false, VertexAttributeType::POSITION));
    InitBuffers(vertices, attribDescriptions, indices);
}

DrawDebugLights::~DrawDebugLights()
{
}

void DrawDebugLights::Draw(ShaderProgram& shader)
{
    shader.UseProgram();
    m_vao.Bind();
    SetupMatrices(shader);
    glDrawElements(GL_TRIANGLES, m_ibo.GetNumIndicies(), GL_UNSIGNED_INT, 0); 
}

void DrawDebugLights::SetupDebugCubes(
    const std::vector<PointLight>& pointLights,
    std::vector<DebugLightVertex>& vertices,
    std::vector<unsigned int>& indices)
{
    for (int i = 0; i < pointLights.size(); i++)
    {
        SetupVertices(pointLights[i], vertices);
        SetupIndices(indices, i);
    }
}

void DrawDebugLights::SetupVertices(const PointLight& pointLights, std::vector<DebugLightVertex>& vertices)
{
    vertices.push_back(DebugLightVertex(glm::vec4(-5.0f, -5.0f, 5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(5.0f, -5.0f, 5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(5.0f, 5.0f, 5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(-5.0f, 5.0f, 5.0f, 0.f) + pointLights.position));

    vertices.push_back(DebugLightVertex(glm::vec4(-5.0f, -5.0f, -5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(5.0f, -5.0f, -5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(5.0f, 5.0f, -5.0f, 0.f) + pointLights.position));
    vertices.push_back(DebugLightVertex(glm::vec4(-5.0f, 5.0f, -5.0f, 0.f) + pointLights.position));
}

void DrawDebugLights::SetupIndices(std::vector<unsigned int>& indices, int i)
{
    std::vector<int> startIndices = {
        0 + i * 8, 1 + i * 8,  2 + i * 8,  // Front face
        0 + i * 8,  2 + i * 8,  3 + i * 8,  // Front face
        1 + i * 8,  5 + i * 8,  6 + i * 8,  // Back face
        1 + i * 8,  6 + i * 8,  2 + i * 8,  // Back face
        4 + i * 8,  0 + i * 8,  3 + i * 8,  // Left face
        4 + i * 8,  3 + i * 8,  7 + i * 8,  // Left face
        5 + i * 8,  4 + i * 8,  7 + i * 8,  // Right face
        5 + i * 8,  7 + i * 8,  6 + i * 8,  // Right face
        3 + i * 8,  2 + i * 8,  6 + i * 8,  // Top face
        3 + i * 8,  6 + i * 8,  7 + i * 8,  // Top face
        4 + i * 8,  5 + i * 8,  1 + i * 8,  // Bottom face
        4 + i * 8,  1 + i * 8,  0 + i * 8  // Bottom face
    };

    indices.insert(indices.end(), startIndices.begin(), startIndices.end());
}