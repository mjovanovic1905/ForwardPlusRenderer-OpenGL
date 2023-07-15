#ifndef _POINT_LIGHT_
#define _POINT_LIGHT_

#include <glm/glm.hpp>

struct PointLight
{    
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float radius;
};

#endif