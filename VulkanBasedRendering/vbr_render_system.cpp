#include "vbr_render_system.hpp"
#include <stdexcept>
#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace vbr
{
	struct SimplePushConstantData {
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
}


vbr::RenderSystem::RenderSystem(VbrDevice& device, VkRenderPass renderPass) : vbrMyDevice(device)
{
	createPipelineLayout();
	createPipeline(renderPass);
}

vbr::RenderSystem::~RenderSystem()
{
	vkDestroyPipelineLayout(vbrMyDevice.getDevice(), pipelineLayout, nullptr);
}

void vbr::RenderSystem::renderGameObjects(VkCommandBuffer buffer, std::vector<VbrGameObject>& gameObjects)
{
    vbrMyPipeline->bind(buffer);

    for (auto& obj : gameObjects)
    {
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
        SimplePushConstantData push{};

        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2();


        vkCmdPushConstants(buffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
            sizeof(SimplePushConstantData),
            &push);

        obj.model->bind(buffer);
        obj.model->draw(buffer);

    }
}

void vbr::RenderSystem::createPipelineLayout()
{
    std::cout << "Application:: creating pipeline layout for logical device\n";

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(vbrMyDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void vbr::RenderSystem::createPipeline(VkRenderPass renderPass)
{
    std::cout << "Creating a graphics pipeline\n";

    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
    std::cout << "Application:: creating pipeline with default pipeline config, a renderpass from swapchain, a pipeline layout and shaders\n";
    PipelineConfigInfo configInfo{};
    VbrPipeline::defaultPipelineConfigInfo(configInfo);
    std::cout << "with some default config info\n";

    configInfo.renderPass = renderPass;
    configInfo.pipelineLayout = pipelineLayout;
    vbrMyPipeline = std::make_unique<VbrPipeline>(
        vbrMyDevice,
        "Shaders/vert.spv",
        "Shaders/frag.spv",
        configInfo);
}
