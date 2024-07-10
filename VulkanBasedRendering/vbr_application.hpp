#pragma once

#include "vbr_device.hpp"
#include "vbr_pipeline.hpp"
#include "vbr_swapchain.hpp"
#include "vbr_window.hpp"
#include "vbr_model.hpp"
#include "vbr_game_object.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace vbr {
	struct SimplePushConstantData {
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
	class VbrApplication {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		VbrApplication();
		~VbrApplication();

		VbrApplication(const VbrApplication&) = delete;
		VbrApplication& operator=(const VbrApplication&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void loadGameObjects();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer buffer);

		VbrWindow vbrWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; // create a window and surface based on device and glfw
		VbrDevice vbrMyDevice{ vbrWindow }; //
		std::unique_ptr<VbrSwapChain> vbrSwapChain;
		std::unique_ptr<VbrPipeline> vbrMyPipeline;
		//std::unique_ptr<VbrModel> vbrMyModel;
		std::vector<VbrGameObject> gameObjects;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}  