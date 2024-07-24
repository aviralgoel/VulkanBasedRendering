#include "vbr_camera.hpp"

void vbr::VbrCamera::setOrthographicsProject(float left, float right, float bottom, float top, float far, float near)
{
	projectionMatrix = glm::mat4(1.0f);
	projectionMatrix[0][0] = 2.0f / (right - left);
	projectionMatrix[1][1] = 2.0f / (bottom - top);
	projectionMatrix[2][2] = 1.0f/ (far-near);
	projectionMatrix[3][0] = -(right + left) / (right - left);
	projectionMatrix[3][1] = -(bottom - bottom) / (bottom - top);
	projectionMatrix[3][2] = -near / (far - near);

}

void vbr::VbrCamera::setPerspectiveProject(float fovy, float aspect, float far, float near)
{
	assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	const float tanHalfFovy = tan(fovy / 2.f);
	projectionMatrix = glm::mat4{ 0.0f };
	projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
	projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	projectionMatrix[2][2] = far / (far - near);
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][2] = -(far * near) / (far - near);

}
