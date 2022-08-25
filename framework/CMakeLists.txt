set(FRAMEWORK_PROJECT_NAME "${PROJECT_NAME}_Framework")
message(STATUS "FRAMEWORK_PROJECT_NAME: ${FRAMEWORK_PROJECT_NAME}")

add_library(${FRAMEWORK_PROJECT_NAME})

target_sources(${FRAMEWORK_PROJECT_NAME}
        PRIVATE application.cpp
        ../bindings/imgui_impl_glfw.cpp
        ../bindings/imgui_impl_vulkan.cpp
        PUBLIC application.hpp logging.hpp
        ../bindings/imgui_impl_glfw.h
        ../bindings/imgui_impl_vulkan.h
)

target_link_libraries(${FRAMEWORK_PROJECT_NAME}
        PUBLIC spdlog::spdlog
        PUBLIC glm
        PRIVATE glfw
        PRIVATE imgui
        PUBLIC "${VULKAN_LIB_DIRS}/${VULKAN_LIB}"
)

# Exports all the include directories needed for this library
target_include_directories(${FRAMEWORK_PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${SPDLOG_INCLUDE_DIRS}
        PUBLIC ${GLFW_INCLUDE_DIRS}
        PUBLIC ${GLM_INCLUDE_DIRS}
        PUBLIC ${IMGUI_INCLUDE_DIRS}
        PUBLIC ${VULKAN_INCLUDE_DIRS})
install(TARGETS ${FRAMEWORK_PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)