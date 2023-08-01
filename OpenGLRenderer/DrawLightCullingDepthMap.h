#pragma once

#include "DrawableObject.h"
#include "ShaderProgram.h"

class Texture;

class DrawLightCullingDepthMap : public DrawableObject
{
public:
	DrawLightCullingDepthMap(Texture& deptMap);

	virtual void Draw(ShaderProgram& shader) override;

private:
	ShaderProgram shaderProgram_;
	Texture& deptMap_;
};

