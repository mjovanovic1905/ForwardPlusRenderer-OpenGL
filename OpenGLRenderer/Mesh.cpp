#include "Mesh.h"

#include <GL/glew.h>
#include <string>

#include "ShaderProgram.h"

Mesh::Mesh()
{
}

void Mesh::Draw(ShaderProgram& shader)
{
    shader.UseProgram();
    SetupMatrices(shader);
    if (material_.diffuseMap.GetId() != 0)
    {
        shader.SetUniformValue("material", material_);
        material_.diffuseMap.BindTexture();
        material_.specularMap.BindTexture();
    }
    
    // Draw mesh
    m_vao.Bind();
    
    if (m_ibo.GetNumIndicies() > 0)
    {
        glDrawElements(GL_TRIANGLES, m_ibo.GetNumIndicies(), GL_UNSIGNED_INT, 0); 
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_vbo.GetNumVertices());
    }
    VAO::Unbid();
    
}

Mesh::~Mesh()
{
}
