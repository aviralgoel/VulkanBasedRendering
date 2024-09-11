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
mat4 inverseView;
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

const float M_PI = 3.14159265359;
void main()
{	
	// discard distance from center
	float distance = sqrt(dot(fragOffset, fragOffset));
	if (distance > 1.0)
	{
		discard;
	}
	outColor = vec4(push.lightColor.xyz, 0.5 * (cos(distance * M_PI) +1) );
}
