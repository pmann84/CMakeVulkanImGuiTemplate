#ifndef VULKANIMGUITEMPLATE_APPLICATION_HPP
#define VULKANIMGUITEMPLATE_APPLICATION_HPP

#include "logging.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
//#include <glm/vec4.hpp>

// TODO: GL event handling
// TODO: Window resize handling

const std::vector<const char*> VULKAN_VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

// TODO: Change this to be a define and then just make all the sections use an #ifdef, 
// then code is guaranteed not to leak into the release build
#ifdef NDEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = true;
#endif

struct application_data
{
    std::string title = "Vulkan ImGui Template";
    uint32_t width = 640;
    uint32_t height = 480;
};

struct vk_queue_family_indices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool is_complete()
    {
        return graphics_family.has_value()
            && present_family.has_value();
    }
};

class application
{
public:
    application(application_data application_props);
    ~application();
    void run();

    virtual void on_update() = 0;
    //    virtual void on_error(int error, const char* description) = 0;
    virtual void on_key_press(int key, int scancode, int action, int mods) {}
    virtual void on_window_close() {}
    virtual void on_window_resize(int width, int height) {}
    virtual void on_char_press(unsigned int keycode) {}
    virtual void on_mouse_button(int button, int action, int mods) {}
    virtual void on_scroll(double xOffset, double yOffset) {}
    virtual void on_cursor_pos_changed(double xPos, double yPos) {}

protected:
    application_data m_props;
    GLFWwindow* m_window;
    VkInstance m_vulkan_instance;
    VkDebugUtilsMessengerEXT m_vulkan_debug_messenger;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
    VkDevice m_logical_device;
    VkQueue m_graphics_queue;
    VkSurfaceKHR m_surface;
    VkQueue m_present_queue;

private:
    void init_window();
    void init_vulkan();
    void init_imgui();
    VkResult create_vulkan_instance();
    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info);
    void setup_debug_messenger();
    bool check_validation_layer_support();
    std::vector<const char*> get_required_extensions();
    void pick_physical_device();
    vk_queue_family_indices find_queue_families(VkPhysicalDevice device);
    void create_logical_device();
    void create_surface();

    void shutdown_window();
    void shutdown_vulkan();
    void shutdown_imgui();
};


#endif //VULKANIMGUITEMPLATE_APPLICATION_HPP