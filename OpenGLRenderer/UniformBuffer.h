#ifndef _UNIFORM_BUFFER_
#define _UNIFORM_BUFFER_

#include <GL/glew.h>

template<typename T>
class UniformBuffer
{
public:
    UniformBuffer();

    void Init(T* data, int numVariables);
    void Bind();
    void InsertData(T*data, int offset);
    unsigned int GetId() const { return id_; }

    // remove this from the template
    // as a new function will be created for every one
    static void UnbindBuffer();

    ~UniformBuffer();

private:
    unsigned int id_;
};

template<typename T>
void UniformBuffer<T>::Init(T* data, int numVariables)
{
    glGenBuffers(1, &id_);
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * numVariables, data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, id_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
UniformBuffer<T>::UniformBuffer()
{
}

template<typename T>
void UniformBuffer<T>::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
}

template<typename T>
void UniformBuffer<T>::InsertData(T* data, int offset)
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(T), data);
}

template<typename T>
void UniformBuffer<T>::UnbindBuffer()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

template<typename T>
UniformBuffer<T>::~UniformBuffer()
{
}

#endif