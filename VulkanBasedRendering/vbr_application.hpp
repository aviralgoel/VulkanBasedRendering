#pragma once

#include "vbr_device.hpp"
#include "vbr_pipeline.hpp"
#include "vbr_swapchain.hpp"
#include "vbr_window.hpp"
#include "vbr_model.hpp"

// std
#include <memory>
#include <vector>

namespace vbr {
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
		void loadModels();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		VbrWindow vbrWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; // create a window and surface based on device and glfw
		VbrDevice vbrMyDevice{ vbrWindow }; //
		std::unique_ptr<VbrSwapChain> vbrSwapChain;
		std::unique_ptr<VbrPipeline> vbrMyPipeline;
		std::unique_ptr<VbrModel> vbrMyModel;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}  