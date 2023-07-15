#ifndef _TEXTURE_ARRAY_
#define _TEXTURE_ARRAY_

class TextureArray
{
public:
    TextureArray();

    void Init(unsigned int resolution, unsigned int textureUnit, int numLevels);
    void Bind();
    unsigned int GetId() const { return id_; }
    unsigned int GetTextureUnit() const { return textureUnit_; }

    ~TextureArray();

private:
    unsigned int id_;
    unsigned int textureUnit_;
};

#endif