#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 modelViewProj;

void main()
{
    gl_Position = modelViewProj * vec4(aPos, 1.0);
}