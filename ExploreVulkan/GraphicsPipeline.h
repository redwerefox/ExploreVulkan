#ifndef _H_INCLUDED_GRAPHICSPIPELINE 
#define _H_INCLUDED_GRAPHICSPIPELINE 

#include <vector>
#include <fstream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GraphicsPipeline
{
	public:
		static std::vector<char> readFile(const std::string& filename);
		static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);

		static void createPipeline(VkDevice device);

	private:

};

#endif