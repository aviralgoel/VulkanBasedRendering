#pragma once
#include<vector>
#include <string>
#include <fstream>
#include <iostream>
namespace vbr
{
	class vbrPipeline {

	public:

		vbrPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
		
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

	private:
	};
}