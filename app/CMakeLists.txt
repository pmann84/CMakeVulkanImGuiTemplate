set(APP_PROJECT_NAME "${PROJECT_NAME}_App")
message(STATUS "APP_PROJECT_NAME: ${APP_PROJECT_NAME}")

add_executable(${APP_PROJECT_NAME} main.cpp)

target_link_libraries(${APP_PROJECT_NAME}
        PUBLIC VulkanImGuiTemplate_Framework
)