#pragma once

#include "vbr_device.hpp"
#include "vbr_pipeline.hpp"
#include "vbr_camera.hpp"

#include "vbr_game_object.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace vbr {

	class RenderSystem {
	public:

		RenderSystem(VbrDevice& device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer buffer, std::vector<VbrGameObject>& gameObjects, const VbrCamera& camera);
		

	private:
		void createPipelineLayout();

		void createPipeline(VkRenderPass renderPass);
		
		

		
		VbrDevice& vbrMyDevice; //
	
		std::unique_ptr<VbrPipeline> vbrMyPipeline;

		VkPipelineLayout pipelineLayout;

	};
}