#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"
#include <vulkan/vulkan.h>


namespace lve
{
#define MAX_LIGHTS 10
	
	struct PointLight {
		glm::vec4 position{}; // ignore w
		glm::vec4 color{}; // w is intensity
	};
	struct FrameInfo {
		int frameIndex;
		VkCommandBuffer commandBuffer;
		float deltaTime;
		LveCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		lve::LveGameObject::Map &gameObjects;

	};
	struct GlobalUbo {
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f };
		glm::vec4 ambientColor{ 1.f, 1.f, 1.f, 0.02f }; // 
		PointLight pointLights[MAX_LIGHTS];
		int numPointLights = 0;
	};
}