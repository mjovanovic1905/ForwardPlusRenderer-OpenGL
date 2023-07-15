#include "VAO.h"

#include "GL/glew.h"

void VAO::Unbid()
{
    glBindVertexArray(0);
}

VAO::VAO()
{
}

void VAO::Init()
{
    glGenVertexArrays(1, &id_);
}

void VAO::Bind()
{
    glBindVertexArray(id_);
}

VAO::~VAO()
{
    glDeleteBuffers(1, &id_);
}
