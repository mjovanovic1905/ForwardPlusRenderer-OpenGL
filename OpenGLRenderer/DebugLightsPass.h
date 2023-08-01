#pragma once

#include "RenderPass.h"
#include "DrawDebugLights.h"

class Camera;

class DebugLightsPass : public RenderPass
{
public:
	DebugLightsPass(const std::vector<PointLight>& pointLights, const Camera& camera);

	virtual void PreDraw() override;
	virtual void PostDraw() override;

private:
	const Camera& camera_;
	DrawDebugLights debugLights_;
};

