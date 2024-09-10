#version 450

const vec2 OFFSETS[6] = vec2[] (
    vec2(-1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0, -1.0),
    vec2( 1.0,  -1.0),
    vec2( -1.0,  1.0),
    vec2( 1.0,  1.0)
);

layout(location = 0) out vec2 fragOffset;

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

	fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = { ubo.view[0][0], ubo.view[1][0], ubo.view[2][0] };
    vec3 cameraUpWorld = { ubo.view[0][1], ubo.view[1][1], ubo.view[2][1] };

    vec3 lightPositionWorld = push.lightPosition.xyz 
    + cameraRightWorld * fragOffset.x * push.lightRadius 
    + cameraUpWorld * fragOffset.y * push.lightRadius;


    gl_Position = ubo.projection * ubo.view * vec4(lightPositionWorld, 1.0);
}
