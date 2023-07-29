#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float nearPlane;
uniform float farPlane;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 *  nearPlane * farPlane) / (farPlane +  nearPlane - z * (farPlane -  nearPlane));	
}

void main()
{
    float depth = LinearizeDepth(texture(depthMap, TexCoords).r) / farPlane; // divide by far for demonstration
    FragColor = vec4(vec3(depth), 1.0);
}