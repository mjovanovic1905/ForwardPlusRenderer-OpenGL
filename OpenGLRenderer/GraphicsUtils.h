#pragma once

#include <functional>

#include "ObjectDrawPass.h"
#include "Camera.h"
#include "DepthMapPass.h"
#include "DirectionalLight.h"
#include "CSMShadowMaps.h"
#include "DepthPrepass.h"
#include "PointLightBuffer.h"
#include "ComputeShader.h"
#include "Model.h"
#include "LightGenerator.h"

class GraphicsUtils
{
public:
	GraphicsUtils(Camera& camera);
	ObjectDrawPass SetupMainPass();
	DepthMapPass SetupCSMDepthMapPass();
	DepthPrepass SetupDepthPrepass();
	ComputeShader SetupLightCulling(DepthPrepass& depthPrepass);
	PointLightBuffer& GetPointLightBuffer() { return pointLightBuffer_; }

private:

	std::vector<ShaderDefine> GetCSMDefines() const;

	Camera& camera_;
	PointLightBuffer pointLightBuffer_;
	DirectionalLight light_;
	CSMShadowMaps csmShadowMaps_;
	std::function<void(ShaderProgram&)> drawFunc_;
	Model sponzaModel_;
	LightGenerator lightGenerator_;
	int workGroupsX_;
	int workGroupsY_;
};

