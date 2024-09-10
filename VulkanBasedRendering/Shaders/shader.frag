#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;
struct PointLight
{
	vec4 position;
	vec4 color;
};

layout(set = 0, binding = 0) uniform GlobalUbo
{
mat4 projection;
mat4 view;
vec4 ambientColor;
PointLight pointLights[10];
int pointLightCount;
} ubo;
layout(push_constant) uniform Push {
	mat4 modelMatrix;
	
	mat3 normalMatrix;
} push;
void main() {
	
	vec3 diffuseLight = ubo.ambientColor.xyz * ubo.ambientColor.w;
	vec3 surfaceNormal = normalize(fragNormalWorld);

	for(int i = 0; i < ubo.pointLightCount; i++)
	{
		PointLight pointLight = ubo.pointLights[i];
		vec3 directionToLight = pointLight.position.xyz - fragPosWorld;
		float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
		float cosAngleIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0);
		vec3 intensity = pointLight.color.xyz * attenuation * pointLight.color.w;
		diffuseLight += intensity * cosAngleIncidence;
	}

	outColor = vec4(fragColor * diffuseLight, 1.0);
}
