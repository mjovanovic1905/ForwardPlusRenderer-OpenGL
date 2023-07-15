#ifndef _INDEX_BUFFER_
#define _INDEX_BUFFER_

#include <vector>

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    void Init(const std::vector<unsigned int>& indices);
    void Bind();
    unsigned long GetNumIndicies() const { return numIndicies_; } 

private:

    unsigned int id_;
    unsigned long numIndicies_;
};

#endif