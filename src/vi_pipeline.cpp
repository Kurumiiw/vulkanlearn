#include "vi_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vi {
    ViPipeline::ViPipeline(ViDevice& device,
                           const std::string& vertPath, 
                           const std::string& fragPath, 
                           const PipelineConfigInfo& configInfo) 
                           : viDevice{device}  {
        createGraphicsPipeline(vertPath, fragPath, configInfo);
    }

    std::vector<char> ViPipeline::readFile(const std::string& filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if(!file.is_open()) {
            throw std::runtime_error("[Vi] Failed to open file at path" + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    void ViPipeline::createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath, const PipelineConfigInfo& configInfo) {
        auto vertCode = readFile(vertPath);
        auto fragCode = readFile(fragPath);
        

        std::cout << "Vertex code size: " << vertCode.size() << std::endl;
        std::cout << "Fragment code size: " << fragCode.size() << std::endl;
    }

    void ViPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(viDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("[Vi] Failed to create shader module");
        }
    }

    PipelineConfigInfo ViPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
        PipelineConfigInfo configInfo{};

        return configInfo;
    }

}