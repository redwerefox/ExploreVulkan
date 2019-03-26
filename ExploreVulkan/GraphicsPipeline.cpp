#include "GraphicsPipeline.h"

 std::vector<char> GraphicsPipeline::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Can't open file");
	}

	size_t filesize = (size_t)file.tellg();
	std::vector<char> buffer(filesize);

	file.seekg(0);
	file.read(buffer.data(), filesize);

	file.close();

	return buffer;
}

 VkShaderModule GraphicsPipeline::createShaderModule(VkDevice device, const std::vector<char>& code) {

	 VkShaderModuleCreateInfo createInfo = {  };
	 createInfo.codeSize = code.size();
	 createInfo.pCode = reinterpret_cast<const uint32_t*> (code.data());
	 createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	 VkShaderModule shaderModule;
	 if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		 throw std::runtime_error("failed to create Shader Module");

 }

 void GraphicsPipeline::createPipeline(VkDevice device)
 {
	 auto vertShaderCode = readFile("shaders/vert.spv");
	 auto fragShaderCode = readFile("shaders/frag.spv");

	 VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);
	 VkShaderModule fragShaderModule = createShaderModule(device, fragShaderCode);

	 VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	 vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	 vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	 vertShaderStageInfo.module = vertShaderModule;
	 vertShaderStageInfo.pName = "main";

	 VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	 fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	 fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	 fragShaderStageInfo.module = fragShaderModule;
	 fragShaderStageInfo.pName = "main";

	 VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
 }
