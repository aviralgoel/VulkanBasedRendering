#include "vbr_application.hpp"
#include "vbr_render_system.hpp"

#include "vbr_camera.hpp"

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
        VbrCamera camera{};

        
        while (!vbrWindow.shouldClose()) {
            glfwPollEvents();

            float aspect = vbrRenderer.getAspectRatio();
            //camera.setOrthographicsProject(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProject(glm::radians(45.0f), aspect, 1.0f, 10.0f);

            if (auto commandBuffer = vbrRenderer.beginFrame())
            {   
                
                vbrRenderer.beginSwapChainRenderPass(commandBuffer);
                myRendersystem.renderGameObjects(commandBuffer, gameObjects, camera);
                vbrRenderer.endSwapChainRenderPass(commandBuffer);
                vbrRenderer.endFrame();
           }
        }

        vkDeviceWaitIdle(vbrMyDevice.getDevice());
    }




    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<VbrModel> createCubeModel(VbrDevice& device, glm::vec3 offset) {
        std::vector<VbrModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<VbrModel>(device, vertices);
    }
    void VbrApplication::loadGameObjects()
    {
        std::shared_ptr<VbrModel> cubeModel = createCubeModel(vbrMyDevice, { 0.0f, 0.0f, 0.0f });

        VbrGameObject cubeObject = VbrGameObject::createGameObject();
        cubeObject.model = cubeModel;
        cubeObject.transform.translation = { 0.0f, 0.0f, 2.5f };
        cubeObject.transform.scale = { 0.5f, 0.5f, 0.5f };
        gameObjects.push_back(std::move(cubeObject));

    }

} // class

