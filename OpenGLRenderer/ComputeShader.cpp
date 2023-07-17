#include <GL/glew.h>

#include "ComputeShader.h"

ComputeShader::ComputeShader(const ShaderData& computeShaderData, int workGroupSizeX, int workGroupSizeY)
	: ShaderProgram()
	, workGroupSizeX_(workGroupSizeX)
	, workGroupSizeY_(workGroupSizeY)
{
	int shader = CreateShader(computeShaderData.sourceCode.c_str(), GL_COMPUTE_SHADER);
	assert(shader != INVALID_SHADER_ID);

	id_ = glCreateProgram();
	glAttachShader(id_, shader);
	glLinkProgram(id_);
	CheckForErrors();

	glDeleteShader(shader);
}

void ComputeShader::Execute()
{
	UseProgram();
	glDispatchCompute(workGroupSizeX_, workGroupSizeY_, 1);
}

void ComputeShader::Wait()
{
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
