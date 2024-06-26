#include "vbr_application.hpp"

// std
#include <array>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace vbr {

    VbrApplication::VbrApplication() {
        std::cout << "Creating a Vulkan Application" << std::endl;
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    VbrApplication::~VbrApplication() 
    { 
        vkDestroyPipelineLayout(vbrMyDevice.getDevice(), pipelineLayout, nullptr); 
    }

    void VbrApplication::run() {
        while (!vbrWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vbrMyDevice.getDevice());
    }

    void VbrApplication::createPipelineLayout() {
        
        std::cout << "Application:: creating pipeline layout for logical device\n";
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

        std::cout << "Creating a graphics pipeline\n";
        assert(vbrSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        std::cout << "Application:: creating pipeline with default pipeline config, a renderpass from swapchain, a pipeline layout and shaders\n";
        PipelineConfigInfo configInfo{};
        VbrPipeline::defaultPipelineConfigInfo(configInfo);
        std::cout << "with some default config info\n";
        
        configInfo.renderPass = vbrSwapChain->getRenderPass();
        configInfo.pipelineLayout = pipelineLayout;
        vbrMyPipeline = std::make_unique<VbrPipeline>(
            vbrMyDevice,
            "Shaders/vert.spv",
            "Shaders/frag.spv",
            configInfo);
    }

    void VbrApplication::createCommandBuffers() {

        std::cout << "Application:: creating commandBuffers for each swapchain image\n";
        commandBuffers.resize(vbrSwapChain->getImageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vbrMyDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(vbrMyDevice.getDevice(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

       
    }
    void VbrApplication::freeCommandBuffers()
    {
        vkFreeCommandBuffers(vbrMyDevice.getDevice(), vbrMyDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
    void VbrApplication::drawFrame() {
        uint32_t imageIndex;
        auto result = vbrSwapChain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain(); return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);

        result = vbrSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vbrWindow.wasWindowResized())
        {
            vbrWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void VbrApplication::loadModels()
    {   
        std::cout << "Loading a 3D Model data" << std::endl;
        std::vector<VbrModel::Vertex> vertices = { 
            {{0.0f,0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-1.0f,0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f,1.0f}, {0.0f, 0.0f, 1.0f}}
        };

        std::cout << "Creating a VbrModel object" << std::endl;
        vbrMyModel = std::make_unique<VbrModel>(vbrMyDevice, vertices);
    }

    void VbrApplication::recreateSwapChain()
    {   
        std::cout << "Recreating/Creating swapchain based on window extents\n";
        auto extent = vbrWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = vbrWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(vbrMyDevice.getDevice());
        if (vbrSwapChain == nullptr)
        {   
            std::cout << "Creating swapchain based on window extents\n";
            vbrSwapChain = std::make_unique<VbrSwapChain>(vbrMyDevice, extent);
        }
        else
        {   
            std::cout << "Recreating swapchain based on window extents\n";
            vbrSwapChain = std::make_unique<VbrSwapChain>(vbrMyDevice, extent, std::move(vbrSwapChain));
            if (vbrSwapChain->getImageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        
        
        createPipeline();
    }

    void VbrApplication::recordCommandBuffer(int imageIndex)
    {
        
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = vbrSwapChain->getRenderPass();
            renderPassInfo.framebuffer = vbrSwapChain->getFrameBuffer(imageIndex);

            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = vbrSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(vbrSwapChain->getSwapChainExtent().width);
            viewport.height = static_cast<float>(vbrSwapChain->getSwapChainExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
            VkRect2D scissor{};
            scissor.offset = { 0,0 };
            scissor.extent = vbrSwapChain->getSwapChainExtent();
            vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

            vbrMyPipeline->bind(commandBuffers[imageIndex]); // bind the pipeline
            vbrMyModel->bind(commandBuffers[imageIndex]);
            vbrMyModel->draw(commandBuffers[imageIndex]);
            vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[imageIndex]);
            if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        
    }

}  // namespace lve