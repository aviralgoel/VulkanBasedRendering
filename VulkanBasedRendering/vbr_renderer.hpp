#pragma once

#include "vbr_device.hpp"
#include "vbr_swapchain.hpp"
#include "vbr_window.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>
namespace vbr {

	class VbrRenderer {
	public:


		VbrRenderer(VbrWindow& window, VbrDevice& device);
		~VbrRenderer();

		VbrRenderer(const VbrRenderer&) = delete;
		VbrRenderer& operator=(const VbrRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return vbrSwapChain->getRenderPass(); }
		float getAspectRatio() const { return vbrSwapChain->extentAspectRatio(); }

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Frame is not yet started\n");
			return commandBuffers[currentFrameIndex]; }

		int getFrameIndex() const { 
			assert(isFrameStarted && "Frame is not yet started, cannot get frame index\n");
			return currentFrameIndex; 
		}

	private:

		void createCommandBuffers();
		void freeCommandBuffers();

		void recreateSwapChain();


		VbrWindow& vbrWindow;
		VbrDevice& vbrMyDevice;
		std::unique_ptr<VbrSwapChain> vbrSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;
		uint32_t currentImageIndex{ 0 };
		int currentFrameIndex;
		bool isFrameStarted {false};
	};
}