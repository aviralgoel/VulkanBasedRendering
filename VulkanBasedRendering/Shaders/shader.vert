
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

struct PointLight
{
	vec4 position;
	vec4 color;
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

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	
	mat3 normalMatrix;
} push;





void main() {

	vec4 worldPos = push.modelMatrix * vec4(position, 1.0);
	gl_Position = ubo.projection * ubo.view * worldPos;

	fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
	fragPosWorld = worldPos.xyz;
	fragColor = color;


}