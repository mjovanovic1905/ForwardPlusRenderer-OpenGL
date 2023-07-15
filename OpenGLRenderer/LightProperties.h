#ifndef _LIGHT_PROPS_
#define _LIGHT_PROPS_

#include <glm/glm.hpp>

struct LightProperties
{
    glm::vec3 position;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


#endif