#pragma once

#include "vbr_device.hpp"
#include "vbr_renderer.hpp"
#include "vbr_window.hpp"
#include "vbr_game_object.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace vbr {
	
	class VbrApplication {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		VbrApplication();
		~VbrApplication();

		VbrApplication(const VbrApplication&) = delete;
		VbrApplication& operator=(const VbrApplication&) = delete;

		void run();

	private:

		void loadGameObjects();

		VbrWindow vbrWindow{ WIDTH, HEIGHT, "Hello Vulkan!" }; // create a window and surface based on device and glfw
		VbrDevice vbrMyDevice{ vbrWindow }; //
		VbrRenderer vbrRenderer{ vbrWindow, vbrMyDevice };


		std::vector<VbrGameObject> gameObjects;
	

	};
}  