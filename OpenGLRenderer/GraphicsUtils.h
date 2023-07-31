#pragma once

#include <functional>

#include "ObjectDrawPass.h"
#include "Camera.h"
#include "CSMDepthPrepass.h"
#include "DirectionalLight.h"
#include "CSMShadowMaps.h"
#include "LightCullingDepthPrepass.h"
#include "PointLightBuffer.h"
#include "ComputeShader.h"
#include "Model.h"
#include "LightGenerator.h"

class GraphicsUtils
{
public:
	GraphicsUtils(Camera& camera);
	ObjectDrawPass SetupMainPass();
	CSMDepthPrepass SetupCSMDepthPrepass();
	LightCullingDepthPrepass SetupLightCullingDepthPrepass();
	ComputeShader SetupLightCullingComputeShader(LightCullingDepthPrepass& depthPrepass);
	PointLightBuffer& GetPointLightBuffer() { return pointLightBuffer_; }

	LightGenerator lightGenerator_;
private:

	std::vector<ShaderDefine> GetCSMDefines() const;
	std::vector<ShaderDefine> GetLightCullingDefines() const;

	Camera& camera_;
	PointLightBuffer pointLightBuffer_;
	DirectionalLight light_;
	CSMShadowMaps csmShadowMaps_;
	std::function<void(ShaderProgram&)> drawFunc_;
	Model sponzaModel_;
	int workGroupsX_;
	int workGroupsY_;
};

