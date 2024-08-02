#include "keyboard_user_controller.hpp"

void lve::KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, LveGameObject& gameObject)
{
	glm::vec3 rotate{ 0 };
	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;
	
	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.rotation += dt * turnSpeed * glm::normalize(rotate);
	}

	// limit pitch value between +/- 85 degree
	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = gameObject.transform.rotation.y;
	const glm::vec3 forwardDir{ sin(yaw), 0.0f, cos(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
	const glm::vec3 upDir{ 0.f, -1.f, 0.f };

	glm::vec3 moveDir{ 0.0f };

	if (glfwGetKey(window, keys.moveForwards) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window, keys.moveBackwards) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.translation += dt * moveSpeed * glm::normalize(moveDir);
	}
}
