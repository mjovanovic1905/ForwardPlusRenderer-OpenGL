#pragma once

#include "RenderPass.h"
#include "ShaderProgram.h"
#include "DrawLightCullingDepthMap.h"

class Texture;

class LightCullingDepthMapDrawPass : public RenderPass
{
public:
	LightCullingDepthMapDrawPass(Texture& deptMap);

private:

	virtual void PreDraw() override;
	virtual void PostDraw() override;

	DrawLightCullingDepthMap drawDebugLights_;

};

