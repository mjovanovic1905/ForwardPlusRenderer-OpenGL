#ifndef _DRAWABLE_OBJECT_
#define _DRAWABLE_OBJECT_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class ShaderProgram;

class DrawableObject
{
public:
    DrawableObject();
    virtual ~DrawableObject() = 0;

    void SetModel(const glm::mat4& model);
    void SetView(const glm::mat4& view);
    void SetProj(const glm::mat4& proj);
    void Translate(const glm::vec3& translation);
    void Scale(const glm::vec3& scale);

    virtual void Draw(ShaderProgram& shader) = 0;

protected:
    template<typename T>
    void InitBuffers(const std::vector<T>& vertices,
        const std::vector<VertexAttributeDescription>& attribDescriptions);

    template<typename T>
    void InitBuffers(const std::vector<T>& vertices,
        const std::vector<VertexAttributeDescription>& attribDescriptions,
        const std::vector<unsigned int>& indices);

    void SetupMatrices(ShaderProgram& shader);

    VAO m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ibo;

private:
    void CalculateMVP();
    void CalculateViewProj();

    glm::mat4 model_;
    glm::mat4 view_;
    glm::mat4 proj_;
    glm::mat4 modelViewProj_;
    glm::mat4 viewProj_;
};

template<typename T>
void DrawableObject::InitBuffers(const std::vector<T>& vertices,
        const std::vector<VertexAttributeDescription>& attribDescriptions)
{
    m_vao.Init();
    m_vao.Bind();

    m_vbo.Init(vertices, attribDescriptions);
    m_vbo.Bind(); 
}

template<typename T>
void DrawableObject::InitBuffers(const std::vector<T>& vertices,
        const std::vector<VertexAttributeDescription>& attribDescriptions,
        const std::vector<unsigned int>& indices)
{
    InitBuffers(vertices, attribDescriptions);
    m_ibo.Init(indices);
    m_ibo.Bind();
}

#endif