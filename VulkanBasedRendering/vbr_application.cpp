#include "vbr_application.hpp"
#include "vbr_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace vbr {

    VbrApplication::VbrApplication() {
        std::cout << "Creating a Vulkan Application" << std::endl;
        loadGameObjects();
    }

    VbrApplication::~VbrApplication()
    {
       
    }

    void VbrApplication::run() {

        RenderSystem myRendersystem{ vbrMyDevice, vbrRenderer.getSwapChainRenderPass() };
        while (!vbrWindow.shouldClose()) {
            glfwPollEvents();
            if (auto commandBuffer = vbrRenderer.beginFrame())
            {
                vbrRenderer.beginSwapChainRenderPass(commandBuffer);
                myRendersystem.renderGameObjects(commandBuffer, gameObjects);
                vbrRenderer.endSwapChainRenderPass(commandBuffer);
                vbrRenderer.endFrame();
           }
        }

        vkDeviceWaitIdle(vbrMyDevice.getDevice());
    }





    void VbrApplication::loadGameObjects()
    {
        std::cout << "Loading a 3D Model data" << std::endl;
        std::vector<VbrModel::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f,0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        std::cout << "Creating a VbrModel object" << std::endl;
        auto vbrMyModel = std::make_shared<VbrModel>(vbrMyDevice, vertices);

        auto triangle = VbrGameObject::createGameObject();
        triangle.model = vbrMyModel;
        triangle.color = { 0.1f, 0.8f, 0.1f };
        triangle.transform2d.translation.x = 0.2f;
        triangle.transform2d.scale = { 1.0f, 1.0f };
        triangle.transform2d.rotation = 0.0f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

} // class

