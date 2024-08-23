#pragma once

#include "lve_device.hpp"

namespace lve {

	class LveBuffer {
		public:
		LveBuffer(LveDevice& device,
			VkDeviceSize instanceSize,
			uint32_t instanceCount,
			VkBufferUsageFlags uageFlags,
			VkMemoryPropertyFlags propertyFlags,
			VkDeviceSize minOffAlignment = 1);

		~LveBuffer();

		LveBuffer(const LveBuffer&) = delete;
		LveBuffer& operator=(const LveBuffer&) = delete;

		VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		void unmap();

		void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

		VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

		void writeToIndex(void* data, int index);
		VkResult flushIndex(int index);
		VkDescriptorBufferInfo descriptorInfoForIndex(int index);
		VkResult invalidateIndex(int index);

		VkBuffer getBuffer() { return buffer; }
		void* getMapppedMemory() { return mapped; }
		uint32_t getInstanceCount() { return instanceCount; }
		VkDeviceSize getInstanceSize() { return instanceSize; }
		VkDeviceSize getAlignmentSize() { return alignmentSize; }
		VkBufferUsageFlags getUsageFlags() { return usageFlags; }
		VkMemoryPropertyFlags getPropertyFlags() { return propertyFlags; }
		VkDeviceSize getBufferSize() { return bufferSize; }

		private:
			static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffAlignment);

			LveDevice& device;
			void* mapped = nullptr;
			VkBuffer buffer = VK_NULL_HANDLE;
			VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

			VkDeviceSize bufferSize;
			uint32_t instanceCount;
			VkDeviceSize instanceSize;
			VkDeviceSize alignmentSize;
			VkBufferUsageFlags usageFlags;
			VkMemoryPropertyFlags propertyFlags;
};
}