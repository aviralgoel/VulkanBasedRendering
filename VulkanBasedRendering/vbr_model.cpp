#include "vbr_model.hpp"

#include <iostream>
#include <cassert>



namespace vbr
{

	VbrModel::VbrModel(VbrDevice& devRef, const std::vector<Vertex>& vertices) : vbrDevice(devRef)
	{	
		std::cout << "Creating a VbrModel object with loaded vertices" << std::endl;
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
		std::cout<<"Creating a vertex buffer with required amount of memory size on our device\n";
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count is atleast three");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		vbrDevice.createBuffer(bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory);

		std::cout << "Copying data from our HOST memory to DEVICE memory" << std::endl;
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
		std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
		attributeDescription[0].binding = 0;
		attributeDescription[0].offset = offsetof(Vertex, position);
		attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[0].location = 0;


		attributeDescription[1].binding = 0;
		attributeDescription[1].offset = offsetof(Vertex, color);
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].location = 1;

		return attributeDescription;
	}

}