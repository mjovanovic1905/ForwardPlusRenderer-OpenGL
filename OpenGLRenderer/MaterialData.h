#ifndef _MATERIAL_DATA_
#define _MATERIAL_DATA_

#include <glm/glm.hpp>

#include "Texture.h"

struct MaterialData
{
    Texture diffuseMap;
    Texture specularMap;
    float shine = 32.f;
};

#endif