#pragma once

#include "lve_camera.hpp"
#include <vulkan/vulkan.h>


namespace lve
{
	struct FrameInfo {
		int frameIndex;
		VkCommandBuffer commandBuffer;
		float deltaTime;
		LveCamera& camera;
	};
}