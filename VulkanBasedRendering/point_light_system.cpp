#pragma once
#include "point_light_system.hpp"
#include <map>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

    struct PointLightPushConstantData {
		glm::vec4 position;
		glm::vec4 color;
        float radius;
	};

    PointLightSystem::PointLightSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetlayout)
        : lveDevice{ device } {
        createPipelineLayout(globalSetlayout);
        createPipeline(renderPass);
    }

    PointLightSystem::~PointLightSystem() {
        vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
    }

    void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetlayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PointLightPushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { globalSetlayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void PointLightSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
        LvePipeline::enableAlphaBlending(pipelineConfig);
        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(
            lveDevice,
            "Shaders/pointlights.vert.spv",
            "Shaders/pointlights.frag.spv",
            pipelineConfig);
    }

    void PointLightSystem::update(FrameInfo& frameInfo, GlobalUbo& ubo)
    {   
        auto rotateLight = glm::rotate(glm::mat4(1.f), frameInfo.deltaTime, glm::vec3(0.f, -1.f, 0.f));
        int lightIndex = 0;
        for (auto& kv : frameInfo.gameObjects) {
            auto& obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

            // copy light to ubo
            ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
            ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);

            lightIndex += 1;
        }
        ubo.numPointLights = lightIndex;
    }

    void PointLightSystem::render(FrameInfo& frameInfo)
    {   
        // sort the light
        std::map<float, LveGameObject::id_t> sorted;
        for (auto& kv : frameInfo.gameObjects) {
            auto& obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            // calculate distance
            auto offset = frameInfo.camera.getCameraPosition() - obj.transform.translation;
            float disSquared = glm::dot(offset, offset);
            sorted[disSquared] = obj.getId();
        }
        lvePipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0,
            1,
            &frameInfo.globalDescriptorSet,
            0,
            nullptr);

        // iterate over the sorted lights
        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
            // use game obj id to find light object
            auto& obj = frameInfo.gameObjects.at(it->second);
			
			PointLightPushConstantData push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
			push.radius = obj.transform.scale.x;

            vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PointLightPushConstantData),
				&push);
			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}


    }

}  // namespace lve
