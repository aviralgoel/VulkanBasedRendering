#pragma once

#include "lve_camera.hpp"
#include "lve_game_object.hpp"
#include <vulkan/vulkan.h>


namespace lve
{
	struct FrameInfo {
		int frameIndex;
		VkCommandBuffer commandBuffer;
		float deltaTime;
		LveCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		lve::LveGameObject::Map &gameObjects;

	};
}