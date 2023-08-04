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
	Texture& deptMap_;
};

