#pragma once

#include "vi_device.hpp"

#include <string>
#include <vector>

namespace vi {
    struct PipelineConfigInfo {

    };

    class ViPipeline {
        public:
        ViPipeline(ViDevice& device, 
                    const std::string& vertPath, 
                    const std::string& fragPath, 
                    const PipelineConfigInfo& configInfo);
        ~ViPipeline() {}

        //ViPipline(const ViPipeline&) = delete;
        //void operator&=(const ViPipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
        
        private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertPath, 
                                    const std::string& fragPath, 
                                    const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        ViDevice& viDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertModule;
        VkShaderModule fragModule;
    };
}