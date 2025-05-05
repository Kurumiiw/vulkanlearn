#include "vi_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vi {
    ViPipeline::ViPipeline(const std::string& vertPath, const std::string& fragPath) {
        createGraphicsPipeline(vertPath, fragPath);
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

    void ViPipeline::createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath) {
        auto vertCode = readFile(vertPath);
        auto fragCode = readFile(fragPath);
        

        std::cout << "Vertex code size: " << vertCode.size() << std::endl;
        std::cout << "Fragment code size: " << fragCode.size() << std::endl;
    }
}