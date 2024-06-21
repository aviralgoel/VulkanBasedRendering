#pragma once

#include "vbr_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

namespace vbr {

	class VbrModel {

	public:

		struct Vertex {
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};


		VbrModel(VbrDevice& devRef, const std::vector<Vertex>& vertices);
		~VbrModel();

		VbrModel(const VbrModel&) = delete;
		VbrModel& operator=(const VbrModel&) = delete;

		void bind(VkCommandBuffer buffer);
		void draw(VkCommandBuffer buffer);




	private:

		VbrDevice& vbrDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		void createVertexBuffers(const std::vector<Vertex>& devices);

	};
}