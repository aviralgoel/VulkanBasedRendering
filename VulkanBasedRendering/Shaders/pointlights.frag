#version 450

layout(location = 0) in vec2 fragOffset;
layout(location = 0) out vec4 outColor;

struct PointLight
{
	vec3 position;
	vec3 color;
};

layout(set = 0, binding = 0) uniform GlobalUbo
{
mat4 projection;
mat4 view;
vec4 ambientColor;
PointLight pointLights[10];
int pointLightCount;
} ubo;

layout(push_constant) uniform PushConstants
{   
    vec4 lightPosition;
    vec4 lightColor;
	float lightRadius;
} push;

void main()
{	
	// discard distance from center
	float distance = length(fragOffset);
	if (distance > 1.0)
	{
		discard;
	}
	outColor = vec4(push.lightColor.xyz, 1.0);
}
