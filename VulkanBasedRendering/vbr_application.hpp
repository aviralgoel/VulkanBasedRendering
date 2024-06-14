#pragma once
#include "vbr_window.hpp"
#include "vbr_pipeline.hpp"



namespace vbr
{
class VbrApplication
{
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	VbrApplication();
	~VbrApplication();

	void run();

private:
	VbrWindow vbrWindow{WIDTH, HEIGHT, "HelloVulkan"};
	vbrPipeline vbrPipeline{ "Shaders/vert.spv", "Shaders/frag.spv" };

};





} // namespace