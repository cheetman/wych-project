#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;
layout (location = 4) in vec4 inTangent;

//layout (set = 0, binding = 0) uniform UBOScene 
//{
//	mat4 projection;
//	mat4 view;
//	vec4 lightPos;
//	vec4 viewPos;
//} uboScene;

layout(push_constant) uniform PushConsts {
	mat4 model;
} primitive;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out mat4 primitivemodel;

void main(void)
{
	outNormal = inNormal;
	primitivemodel = primitive.model;

	gl_Position = vec4(inPos.xyz, 1.0);
//	gl_Position = uboScene.projection * uboScene.view * primitive.model * vec4(inPos.xyz, 1.0);
}