
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo
{
mat4 projectionViewMatrix;
vec4 ambientColor;
vec3 lightPosition;
vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	
	mat3 normalMatrix;
} push;





void main() {

	vec4 worldPos = push.modelMatrix * vec4(position, 1.0);
	gl_Position = ubo.projectionViewMatrix * worldPos;

	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);
	vec3 directionToLight = ubo.lightPosition - worldPos.xyz;
	
	float squaredDistance = dot(directionToLight, directionToLight);
	float attenuation = 1.0 / squaredDistance;

	directionToLight = normalize(directionToLight);

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation ;
	vec3 ambientColor = ubo.ambientColor.xyz * ubo.ambientColor.w;
	vec3 diffuseColor = lightColor * max(0, dot(normalWorldSpace, directionToLight));
	


	fragColor = color * (ambientColor + diffuseColor);


}