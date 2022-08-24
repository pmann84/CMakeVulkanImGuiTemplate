#include "application.hpp"

//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

static void error_callback(int error, const char* description)
{
    logging::error("GLFW Error ({0}): {1}", error, description);
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
    createVulkanInstance();
}

void application::init_imgui()
{
}

VkResult application::createVulkanInstance()
{
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

    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;

    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;

    logging::debug("{0} extensions required for GLFW.", glfw_extension_count);

    create_info.enabledLayerCount = 0;

    if (vkCreateInstance(&create_info, nullptr, &m_vulkan_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Could not create Vulkan instance.");
    }
}

void application::shutdown_window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void application::shutdown_vulkan()
{
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