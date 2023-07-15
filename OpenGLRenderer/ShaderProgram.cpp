#include "ShaderProgram.h"

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "MaterialData.h"
#include "LightProperties.h"
#include "DirectionalLight.h"
#include "PointLight.h"

std::string ShaderData::ToString() const
{
    std::string allDefines;
    for (const ShaderDefine& define : defines)
    {
        allDefines.append(define.ToString());
    }
    std::string findStr = "#version 450 core";
    size_t mainPos = sourceCode.find(findStr) + findStr.size();
    std::string res = sourceCode;
    res.insert(mainPos, allDefines);
    return res;
}

ShaderProgram::ShaderProgram(/* args */)
{
}

unsigned int ShaderProgram::CreateShader(const char* shaderSoruce, unsigned int shaderType)
{
    unsigned int shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSoruce, NULL);
    glCompileShader(shaderId);

    int  success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
        return INVALID_SHADER_ID;
    }

    return shaderId;
}

bool ShaderProgram::Init(
    const ShaderData* vertexData,
    const ShaderData* fragmentData,
    const ShaderData* geometryData)
{
    unsigned int vertexShader = CreateShader(vertexData->ToString().c_str(), GL_VERTEX_SHADER);
    if (vertexShader == INVALID_SHADER_ID)
    {
        return false;
    }

    unsigned int fragmentShader = CreateShader(fragmentData->ToString().c_str(), GL_FRAGMENT_SHADER);
    if (fragmentShader == INVALID_SHADER_ID)
    {
        return false;
    }

    unsigned int geomertyShader = 0;
    if (geometryData != nullptr)
    {
        geomertyShader = CreateShader(geometryData->ToString().c_str(), GL_GEOMETRY_SHADER);
        if (geomertyShader == INVALID_SHADER_ID)
        {
            return false;
        }
    }

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    if (geometryData != nullptr)
    {
        glAttachShader(shaderProgram_, geomertyShader);
    }
    glLinkProgram(shaderProgram_);
    int  success;
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryData != nullptr)
    {
        glDeleteShader(geomertyShader);
    }

    return true;
}

void ShaderProgram::UseProgram()
{
    glUseProgram(shaderProgram_);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const glm::vec4& value)
{
    int uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    glUniform4f(uniformLocation, value[0], value[1], value[2], value[3]);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const glm::vec3& value)
{
    int uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    glUniform3f(uniformLocation, value[0], value[1], value[2]);
}

void ShaderProgram::SetUniformValue(const char* uniformName, int value)
{
    int uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    glUniform1i(uniformLocation, value);
}

void ShaderProgram::SetUniformValue(const char* uniformName, float value)
{
    int uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    glUniform1f(uniformLocation, value);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const glm::mat4& matrix)
{
    int uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::SetUniformValue(const char* uniformName, MaterialData& material)
{
    std::string baseName = uniformName;

    material.diffuseMap.BindTexture();    
    SetUniformValue((baseName + ".diffuseMap").c_str(), (int)material.diffuseMap.GetTextureUnit());
    SetUniformValue((baseName + ".specularMap").c_str(), (int)material.specularMap.GetTextureUnit());
    SetUniformValue((baseName + ".shine").c_str(), material.shine);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const LightProperties& light)
{
    std::string baseName = uniformName;
    
    SetUniformValue((baseName + ".position").c_str(), light.position);
    SetUniformValue((baseName + ".ambient").c_str(), light.ambient);
    SetUniformValue((baseName + ".diffuse").c_str(), light.diffuse);
    SetUniformValue((baseName + ".specular").c_str(), light.specular);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const DirectionalLight& light)
{
    std::string baseName = uniformName;
    
    SetUniformValue((baseName + ".direction").c_str(), light.direction);
    SetUniformValue((baseName + ".ambient").c_str(), light.ambient);
    SetUniformValue((baseName + ".diffuse").c_str(), light.diffuse);
    SetUniformValue((baseName + ".specular").c_str(), light.specular);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const PointLight& light)
{
    std::string baseName = uniformName;
    
    SetUniformValue((baseName + ".position").c_str(), light.position);

    SetUniformValue((baseName + ".constant").c_str(), light.constant);
    SetUniformValue((baseName + ".linear").c_str(), light.linear);
    SetUniformValue((baseName + ".quadratic").c_str(), light.quadratic);

    SetUniformValue((baseName + ".ambient").c_str(), light.ambient);
    SetUniformValue((baseName + ".diffuse").c_str(), light.diffuse);
    SetUniformValue((baseName + ".specular").c_str(), light.specular);

    SetUniformValue((baseName + ".radius").c_str(), light.radius);
}

void ShaderProgram::SetUniformValue(const char* uniformName, const std::vector<PointLight>& lights)
{
    for (int i = 0; i < lights.size(); i++)
    {
        std::string name = uniformName;
        name.append("[").append(std::to_string(i)).append("]");
        SetUniformValue(name.c_str(), lights[i]);
    }
}

void ShaderProgram::SetUniformBuffer(const char* uniformBufferName, int binding)
{
    unsigned int uboIndex = glGetUniformBlockIndex(shaderProgram_, uniformBufferName);   
    glUniformBlockBinding(shaderProgram_, uboIndex, binding);
}

ShaderProgram::~ShaderProgram()
{
}
