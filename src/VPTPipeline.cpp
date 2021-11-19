#include "VPTPipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>
namespace VPT {
    VPTPipeline::VPTPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
        createGraphicsPipeline(vertFilePath,fragFilePath);
    }
    std::vector<char> VPTPipeline::readFile(const std::string& filePath){
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};
        if(!file.is_open()){
            throw std::runtime_error("failed to open file: " + filePath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
    void VPTPipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout<< vertCode.size() << '\n';
        std::cout<< fragCode.size() << '\n';
    }

}