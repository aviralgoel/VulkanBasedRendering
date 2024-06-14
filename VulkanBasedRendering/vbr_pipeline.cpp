#include "vbr_pipeline.hpp"

namespace vbr {
	vbrPipeline::vbrPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath);
	}
	// reads shader files and returns a byte buffer with shader data
	std::vector<char> vbr::vbrPipeline::readFile(const std::string& filePath)
	{	
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file");
		}

		size_t fileSize = static_cast<size_t> (file.tellg());
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void vbr::vbrPipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertShaderCode = readFile(vertFilePath);
		auto fragShaderCode = readFile(fragFilePath);

		std::cout << "Vertex shader code size: " << vertShaderCode.size() << std::endl;
		std::cout << "Fragment shader code size: " << fragShaderCode.size() << std::endl;
	}

}


