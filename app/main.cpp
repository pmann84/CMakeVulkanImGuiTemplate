//#include <application.hpp>
#include <logging.hpp>
//class my_app : public application
//{
//public:
//    my_app(application_data props) : application(props)
//    {
//    }
//
//    // This function is run every frame
//    void on_update() override
//    {
//        logging::debug("Updating my app!");
//    }
//};

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

int main()
{
    logging::init(spdlog::level::debug);
    logging::info("Starting Vulkan Window");
    //application_data props{ "My Amazing App", 1024, 768 };
    //my_app app(props);
    //app.run();

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    logging::info("{0} extensions supported!", extensionCount);

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}