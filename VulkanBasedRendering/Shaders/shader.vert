
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;

layout(push_constant) uniform Push {
	mat4 transform; // projection * view * model
	//mat4 normalMatrix; // model
	mat4 modelMatrix;
} push;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT_INTENSITY = 0.12;

void main() {

  gl_Position = push.transform * vec4(position, 1.0);
  mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
  vec3 normalWorldSpace = normalize(normalMatrix * normal);
  //vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);
  float lightIntensity = AMBIENT_INTENSITY + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);
  fragColor = lightIntensity*color;


  fragNormal = normal;
  fragTexCoord = texCoord;
}