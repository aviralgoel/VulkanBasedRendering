#include "vbr_model.hpp"

#include <cassert>



namespace vbr
{

	VbrModel::VbrModel(VbrDevice& devRef, const std::vector<Vertex>& vertices) : vbrDevice(devRef)
	{
		createVertexBuffers(vertices);
	}

	VbrModel::~VbrModel()
	{
		vkDestroyBuffer(vbrDevice.getDevice(), vertexBuffer, nullptr);
		vkFreeMemory(vbrDevice.getDevice(), vertexBufferMemory, nullptr);
	}

	void VbrModel::bind(VkCommandBuffer buffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(buffer, 0, 1, buffers, offsets);


	}

	void VbrModel::draw(VkCommandBuffer buffer)
	{
		vkCmdDraw(buffer, vertexCount, 1, 0, 0);
	}

	void VbrModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count is atleast three");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		vbrDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory);

		void* data;
		vkMapMemory(vbrDevice.getDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(vbrDevice.getDevice(), vertexBufferMemory);


	}

	std::vector<VkVertexInputBindingDescription> VbrModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> VbrModel::Vertex::getAttributeDescriptions()
	{	
		std::vector<VkVertexInputAttributeDescription> attributeDescription(1);
		attributeDescription[0].binding = 0;
		attributeDescription[0].offset = 0,
		attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription[0].location = 0;

		return attributeDescription;
	}

}