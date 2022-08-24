#include "application.hpp"

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

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
        logging::debug("Validation layer: {0}", callback_data->pMessage);
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
    //const char* glsl_version = "#version 130";
    //ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    //ImGui_ImplOpenGL3_Init(glsl_version);
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
    pick_physical_device();
}

void application::init_imgui()
{
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
    auto is_device_suitable = [this](VkPhysicalDevice device) {
        vk_queue_family_indices indices = find_queue_families(device);
        return indices.is_complete();
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
        if (is_device_suitable(device))
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

        if (indices.is_complete())
        {
            break;
        }
        i++;
    }

    return indices;
}

void application::shutdown_window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void application::shutdown_vulkan()
{
    if (enable_validation_layers) 
    {
        destroy_debug_utils_messenger_ext(m_vulkan_instance, m_vulkan_debug_messenger, nullptr);
    }

    vkDestroyInstance(m_vulkan_instance, nullptr);
}

void application::shutdown_imgui()
{
    ////ImGui_ImplOpenGL3_Shutdown();
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