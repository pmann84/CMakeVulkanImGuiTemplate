#include "application.hpp"
#include <set>
#include <algorithm>

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_vulkan.h"

static void error_callback(int error, const char* description)
{
    logging::error("GLFW Error ({0}): {1}", error, description);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data)
{
    // Can get info about the application here using this
    //application& app = *(application*)user_data;

    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) 
    {
        logging::error("Validation layer: {0}", callback_data->pMessage);
    } 
    else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        logging::warn("Validation layer: {0}", callback_data->pMessage);
    }
    else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        logging::info("Validation layer: {0}", callback_data->pMessage);
    }
    else if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        //logging::debug("Validation layer: {0}", callback_data->pMessage);
    }
    return VK_FALSE;
}

VkResult create_debug_utils_messenger_ext(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* debug_messenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, create_info, allocator, debug_messenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroy_debug_utils_messenger_ext(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks* allocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debug_messenger, allocator);
    }
}

application::application(application_data application_props) : m_props(application_props)
{
    logging::init(spdlog::level::debug);
    logging::debug("Starting application...");

    init_window();
    init_vulkan();
    init_imgui();
    
    //gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    //glfwSwapInterval(1);
}

void application::run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        // Swap buffer and poll for events
        glfwPollEvents();

    //    // Start the Dear ImGui frame
    //    //ImGui_ImplOpenGL3_NewFrame();
    //    //ImGui_ImplGlfw_NewFrame();
    //    //ImGui::NewFrame();

    //    //bool show_demo_window = true;
    //    //ImGui::ShowDemoWindow(&show_demo_window);

    //    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //    //glClear(GL_COLOR_BUFFER_BIT);

        // Code to run every frame
        on_update();

    //    //ImGui::Render();
    //    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //glfwSwapBuffers(m_window);
    }
}

void application::init_window()
{
    if (!glfwInit())
    {
        // Initialization failed
        throw std::runtime_error("GLFW Initialisation failed.");
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_props.width, m_props.height, m_props.title.c_str(), NULL, NULL);
    if (!m_window)
    {
        // Window or context creation failed
        glfwTerminate();
        throw std::runtime_error("GLFW Window creation failed.");
    }

    if (!glfwVulkanSupported())
    {
        throw std::runtime_error("GLFW does not support Vulkan!");
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        application& app = *(application*)glfwGetWindowUserPointer(window);
        // TODO: Consider converting this into something more sane?
        app.on_key_press(key, scancode, action, mods);
        });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        application& app = *(application*)glfwGetWindowUserPointer(window);
        app.on_window_close();
        });

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        application& app = *(application*)glfwGetWindowUserPointer(window);
        app.m_props.width = width;
        app.m_props.height = height;
        app.on_window_resize(width, height);
        });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
        {
            application& app = *(application*)glfwGetWindowUserPointer(window);
            app.on_char_press(keycode);
        });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            application& app = *(application*)glfwGetWindowUserPointer(window);
            app.on_mouse_button(button, action, mods);
        });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            application& app = *(application*)glfwGetWindowUserPointer(window);
            app.on_scroll(xOffset, yOffset);
        });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
        {
            application& app = *(application*)glfwGetWindowUserPointer(window);
            app.on_cursor_pos_changed(xPos, yPos);
        });

    //glfwMakeContextCurrent(m_window);
}

void application::init_vulkan()
{
    create_vulkan_instance();
    setup_debug_messenger();
    create_surface();
    pick_physical_device();
    create_logical_device();
    create_swap_chain();
}

void application::init_imgui()
{
    // Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    //ImGui_ImplGlfw_InitForVulkan(m_window, true);
}

VkResult application::create_vulkan_instance()
{
    if (enable_validation_layers && !check_validation_layer_support())
    {
        throw std::runtime_error("Validation layers requested but not available.");
    };

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = m_props.title.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;


    auto extensions = get_required_extensions();
    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();

    logging::debug("Loading {0} Vulkan extensions.", extensions.size());

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (enable_validation_layers)
    {
        create_info.enabledLayerCount = VULKAN_VALIDATION_LAYERS.size();
        create_info.ppEnabledLayerNames = VULKAN_VALIDATION_LAYERS.data();

        populate_debug_messenger_create_info(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
    }
    else
    {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &m_vulkan_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Could not create Vulkan instance.");
    }
}

void application::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info)
{
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    create_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_callback;
    create_info.pUserData = this; // Optional used to specify user data to pass to the call back
}

void application::setup_debug_messenger()
{
    if (!enable_validation_layers) return;

    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    populate_debug_messenger_create_info(create_info);

    if (create_debug_utils_messenger_ext(m_vulkan_instance, &create_info, nullptr, &m_vulkan_debug_messenger) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

bool application::check_validation_layer_support()
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : VULKAN_VALIDATION_LAYERS)
    {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers)
        {
            if (strcmp(layer_name, layer_properties.layerName) == 0)
            {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> application::get_required_extensions()
{
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (enable_validation_layers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void application::pick_physical_device()
{
    auto check_device_extension_support = [](VkPhysicalDevice device) {
        uint32_t extension_count;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

        std::set<std::string> required_extensions(VULKAN_DEVICE_EXTENSIONS.begin(), VULKAN_DEVICE_EXTENSIONS.end());

        // Leaves us with any extensions we dont have available and so means the device is not suitable
        for (const auto& extension : available_extensions)
        {
            required_extensions.erase(extension.extensionName);
        }
        return required_extensions.empty();
    };

    auto is_device_suitable = [this, check_device_extension_support](VkPhysicalDevice device, VkSurfaceKHR surface) {
        vk_queue_family_indices indices = find_queue_families(device);

        bool extensions_supported = check_device_extension_support(device);

        bool swap_chain_adequate = false;
        if (extensions_supported)
        {
            vk_swap_chain_support_details swap_chain_support = query_swap_chain_support(device, surface);
            swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
        }

        return indices.is_complete() && extensions_supported && swap_chain_adequate;
    };

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_vulkan_instance, &device_count, nullptr);

    if (device_count == 0)
    {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_vulkan_instance, &device_count, devices.data());

    for (const auto& device : devices)
    {
        if (is_device_suitable(device, m_surface))
        {
            m_physical_device = device;
            break;
        }
    }

    if (m_physical_device == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to a suitable GPU!");
    }
}

vk_queue_family_indices application::find_queue_families(VkPhysicalDevice device)
{
    vk_queue_family_indices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;
    for (const auto& queue_family : queue_families)
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphics_family = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &present_support);
        if (present_support)
        {
            indices.present_family = i;
        }

        if (indices.is_complete())
        {
            break;
        }
        i++;
    }

    return indices;
}

vk_swap_chain_support_details application::query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    vk_swap_chain_support_details details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t format_count; 
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

    if (format_count != 0)
    {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0)
    {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
    }
    return details;
}

VkSurfaceFormatKHR application::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
    for (const auto& available_format : available_formats) 
    {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB 
            && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return available_format;
        }
    }
    return available_formats[0];
}

VkPresentModeKHR application::choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
    for (const auto& available_present_mode : available_present_modes) 
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) 
        {
            return available_present_mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D application::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        VkExtent2D actual_extent = {
            width, height
        };

        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actual_extent;
    }
}

void application::create_logical_device()
{
    vk_queue_family_indices indices = find_queue_families(m_physical_device);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = { 
        indices.graphics_family.value(), 
        indices.present_family.value() 
    };
    float queue_priority = 1.0f;

    for (uint32_t queue_family : unique_queue_families)
    {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos.data();
    device_create_info.queueCreateInfoCount = queue_create_infos.size();
    device_create_info.pEnabledFeatures = &device_features;

    device_create_info.enabledExtensionCount = VULKAN_DEVICE_EXTENSIONS.size();
    device_create_info.ppEnabledExtensionNames = VULKAN_DEVICE_EXTENSIONS.data();
    device_create_info.enabledLayerCount = 0;

    if (enable_validation_layers) 
    {
        device_create_info.enabledLayerCount = VULKAN_VALIDATION_LAYERS.size();
        device_create_info.ppEnabledLayerNames = VULKAN_VALIDATION_LAYERS.data();
    }

    if (vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_logical_device))
    {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_logical_device, indices.graphics_family.value(), 0, &m_graphics_queue);
    vkGetDeviceQueue(m_logical_device, indices.present_family.value(), 0, &m_present_queue);
}

void application::create_surface()
{
    if (glfwCreateWindowSurface(m_vulkan_instance, m_window, nullptr, &m_surface))
    {
        throw std::runtime_error("Failed to create window surface.");
    }
}

void application::create_swap_chain()
{
    vk_swap_chain_support_details swap_chain_support = query_swap_chain_support(m_physical_device, m_surface);
    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount)
    {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swap_create_info{};
    swap_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_create_info.surface = m_surface;
    swap_create_info.minImageCount = image_count;
    swap_create_info.imageFormat = surface_format.format;
    swap_create_info.imageColorSpace = surface_format.colorSpace;
    swap_create_info.imageExtent = extent;
    swap_create_info.imageArrayLayers = 1;
    swap_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    vk_queue_family_indices indices = find_queue_families(m_physical_device);
    uint32_t queueFamilyIndices[] = { indices.graphics_family.value(), indices.present_family.value() };

    if (indices.graphics_family != indices.present_family) {
        swap_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swap_create_info.queueFamilyIndexCount = 2;
        swap_create_info.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swap_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swap_create_info.queueFamilyIndexCount = 0; // Optional
        swap_create_info.pQueueFamilyIndices = nullptr; // Optional
    }

    swap_create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    swap_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_create_info.presentMode = present_mode;
    swap_create_info.clipped = VK_TRUE;
    swap_create_info.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(m_logical_device, &swap_create_info, nullptr, &m_swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_logical_device, m_swap_chain, &image_count, nullptr);
    m_swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_logical_device, m_swap_chain, &image_count, m_swap_chain_images.data());

    m_swap_chain_image_format = surface_format.format;
    m_swap_chain_extent = extent;
}

void application::shutdown_window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void application::shutdown_vulkan()
{
    vkDestroySwapchainKHR(m_logical_device, m_swap_chain, nullptr);
    vkDestroyDevice(m_logical_device, nullptr);

    if (enable_validation_layers) 
    {
        destroy_debug_utils_messenger_ext(m_vulkan_instance, m_vulkan_debug_messenger, nullptr);
    }

    vkDestroySurfaceKHR(m_vulkan_instance, m_surface, nullptr);
    vkDestroyInstance(m_vulkan_instance, nullptr);
}

void application::shutdown_imgui()
{
    ////ImGui_ImplVulkan_Shutdown();
    ////ImGui_ImplGlfw_Shutdown();
    ////ImGui::DestroyContext();
}

application::~application()
{
    logging::debug("Closing application...");

    shutdown_imgui();
    shutdown_vulkan();
    shutdown_window();
}