#version 450

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (set = 0, binding = 1) uniform UBOScene 
{
	mat4 projection;
	mat4 view;
	vec4 lightPos;
	vec4 viewPos;
} uboScene;

layout (set = 2, binding = 1) uniform Custom 
{
	float ambient;
	float specularExponent; // 用不到只是占位子
	float normalLength;
} custom;


layout (location = 0) in vec3 inNormal[];
layout (location = 1) in mat4 primitivemodel[];

layout (location = 0) out vec3 outColor;

void main(void)
{	
	float normalLength = custom.normalLength;
	for(int i=0; i<gl_in.length(); i++)
	{
		vec3 pos = gl_in[i].gl_Position.xyz;
		vec3 normal = inNormal[i].xyz;

		gl_Position =  uboScene.projection * uboScene.view * primitivemodel[i] * vec4(pos.xyz, 1.0);
		outColor = vec3(1.0, 0.0, 0.0);
		EmitVertex();
		
		gl_Position =  uboScene.projection * uboScene.view * primitivemodel[i] * vec4(pos.xyz + normal * normalLength, 1.0);
		outColor = vec3(0.0, 0.0, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}