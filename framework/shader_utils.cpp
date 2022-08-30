#include "shader_utils.hpp"

#include <fstream>

std::vector<char> shaders::read_shader(const std::string& shader_filename)
{
    // Read the file to the end so we can tell how big it is
    std::ifstream file(shader_filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot find shader file: " + shader_filename);
    }

    size_t shader_file_size = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(shader_file_size);

    file.seekg(0);
    file.read(buffer.data(), shader_file_size);
    file.close();

    return buffer;
}

VkShaderModule shaders::create_module(const std::vector<char> shader, const VkDevice& device)
{
    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(shader.data());

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module.");
    }

    return shader_module;
}
