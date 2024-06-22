#include "vbr_application.hpp"

// std
#include <array>
#include <stdexcept>
#include <iostream>

namespace vbr {

    VbrApplication::VbrApplication() {
        std::cout << "Creating a vulkan application\n";
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    VbrApplication::~VbrApplication() 
    { 
        vkDestroyPipelineLayout(vbrMyDevice.getDevice(), pipelineLayout, nullptr); 
    }

    void VbrApplication::run() {
        while (!lveWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vbrMyDevice.getDevice());
    }

    void VbrApplication::createPipelineLayout() {
        std::cout << "Application:: creating pipeline layout\n";
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(vbrMyDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void VbrApplication::createPipeline() {
        std::cout << "Application:: creating pipeline with default pipeline config, a renderpass from swapchain, a pipeline layout and shaders\n";
        auto pipelineConfig =
            VbrPipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
        pipelineConfig.renderPass = lveSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        vbrMyPipeline = std::make_unique<VbrPipeline>(
            vbrMyDevice,
            "Shaders/vert.spv",
            "Shaders/frag.spv",
            pipelineConfig);
    }

    void VbrApplication::createCommandBuffers() {
        std::cout << "Application:: creating commandBuffers for each swapchain image\n";
        commandBuffers.resize(lveSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vbrMyDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(vbrMyDevice.getDevice(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = lveSwapChain.getRenderPass();
            renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vbrMyPipeline->bind(commandBuffers[i]); // bind the pipeline
            vbrMyModel->bind(commandBuffers[i]);
            vbrMyModel->draw(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }
    void VbrApplication::drawFrame() {
        uint32_t imageIndex;
        auto result = lveSwapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void VbrApplication::loadModels()
    {
        std::vector<VbrModel::Vertex> vertices = { 
            {{0.0f,0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-1.0f,0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f,1.0f}, {0.0f, 0.0f, 1.0f}}
        };

        vbrMyModel = std::make_unique<VbrModel>(vbrMyDevice, vertices);
    }

}  // namespace lve