#version 450 core

// NUM_CSM_PLANES is a static define that needs to be supplied to the shader
// via the ShaderData struct before building the shader itself


layout(triangles, invocations = NUM_CSM_PLANES) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[NUM_CSM_PLANES];
};

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}  