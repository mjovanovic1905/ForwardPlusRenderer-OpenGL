#ifndef _DIR_LIGHT_
#define _DIR_LIGHT_

#include <glm/glm.hpp>

struct DirectionalLight
{
    glm::vec3 direction;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif