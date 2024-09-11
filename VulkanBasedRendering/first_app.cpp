#include "first_app.hpp"

#include "keyboard_user_controller.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include "point_light_system.hpp"
#include "lve_buffer.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>
#include <numeric>

namespace lve {




    FirstApp::FirstApp() {

        globalPool =
            LveDescriptorPool::Builder(lveDevice)
            .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {

        std::vector<std::unique_ptr<LveBuffer>> uniformBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < LveSwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
			uniformBuffers[i] = std::make_unique<LveBuffer>(lveDevice,
															 sizeof(GlobalUbo),
                1,
															 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
															 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uniformBuffers[i]->map();
		}

        auto globalSetLayout =
            LveDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uniformBuffers[i]->descriptorInfo();
            LveDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }



        SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };


        LveCamera camera{};

        auto viewerObject = LveGameObject::createGameObject();
        viewerObject.transform.translation = { 0.f, 0.f, -2.5f };
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!lveWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = lveRenderer.beginFrame()) {
                int frameIndex = lveRenderer.getFrameIndex();

                FrameInfo frameInfo{ frameIndex, commandBuffer, frameTime, camera, globalDescriptorSets[frameIndex], gameObjectsMap};
                // update
                GlobalUbo globalUbo{};
                globalUbo.projection = camera.getProjection();
                globalUbo.view = camera.getView();
                globalUbo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, globalUbo);
                uniformBuffers[frameIndex]->writeToBuffer(&globalUbo);
                uniformBuffers[frameIndex]->flush();



                // render
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                // order matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");
        auto flatVase = LveGameObject::createGameObject();
        flatVase.model = lveModel;
        flatVase.transform.translation = { -.5f, .5f, 0.0f };
        flatVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjectsMap.emplace(flatVase.getId(), std::move(flatVase));



        lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
        auto smoothVase = LveGameObject::createGameObject();
        smoothVase.model = lveModel;
        smoothVase.transform.translation = { .5f, .5f, 0.0f };
        smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjectsMap.emplace(smoothVase.getId(), std::move(smoothVase));

        lveModel = LveModel::createModelFromFile(lveDevice, "models/floor.obj");
        auto floor = LveGameObject::createGameObject();
        floor.model = lveModel;
        floor.transform.translation = { 0.f, 0.5f, 0.0f };
        floor.transform.scale = { 2.f, 1.f, 2.f };
        gameObjectsMap.emplace(floor.getId(),std::move(floor));

      
      
        std::vector<glm::vec3> lightColors{
        {1.f, .1f, .1f},
        {.1f, .1f, 1.f},
        {.1f, 1.f, .1f},
        {1.f, 1.f, .1f},
        {.1f, 1.f, 1.f},
        {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++)
        {
            auto pointLight = LveGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { 0.f, -1.f, 0.f });

            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            gameObjectsMap.emplace(pointLight.getId(), std::move(pointLight));
        }


       

    }

}  // namespace lve
