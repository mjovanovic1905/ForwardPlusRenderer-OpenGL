#include "DrawableObject.h"
#include "GL/gl.h"

#include "ShaderProgram.h"


DrawableObject::DrawableObject()
: model_(1.f)
, view_(1.f)
, proj_(1.f)
{

}

DrawableObject::~DrawableObject()
{
}

void DrawableObject::SetModel(const glm::mat4& model)
{
    model_ = model;
}

void DrawableObject::SetView(const glm::mat4& view)
{
    view_ = view;
}

void DrawableObject::SetProj(const glm::mat4& proj)
{
    proj_ = proj;
}

void DrawableObject::CalculateMVP()
{
    modelViewProj_ = viewProj_ * model_;
}

void DrawableObject::CalculateViewProj()
{
    viewProj_ = proj_ * view_;
}

void DrawableObject::Translate(const glm::vec3& translation)
{
    model_ = glm::translate(model_, translation);
}

void DrawableObject::Scale(const glm::vec3& scale)
{
    model_ = glm::scale(model_, scale);
}

void DrawableObject::SetupMatrices(ShaderProgram& shader)
{
    CalculateViewProj();
    CalculateMVP();
    shader.SetUniformValue("modelViewProj", modelViewProj_);
    shader.SetUniformValue("viewProj", viewProj_);
    shader.SetUniformValue("model", model_);
}