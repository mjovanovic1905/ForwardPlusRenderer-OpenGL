#include "IndexBuffer.h"
#include "EngineUtils.h"

#include "GL/glew.h"
#include <algorithm>

IndexBuffer::IndexBuffer()
{
}

void IndexBuffer::Init(const std::vector<unsigned int>& indices)
{
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW); 
    numIndicies_ = indices.size();
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

IndexBuffer::~IndexBuffer()
{
}
