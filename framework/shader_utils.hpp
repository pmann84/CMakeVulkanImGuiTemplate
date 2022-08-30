#include <vulkan/vulkan.h>

#include <vector>
#include <string>

namespace shaders {
    std::vector<char> read_shader(const std::string& shader_filename);

    VkShaderModule create_module(const std::vector<char> shader, const VkDevice& device);
}
