add_subdirectory(vendor/spdlog)
# Add header only spdlog directory
set(SPDLOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)

add_subdirectory(vendor/glm)

# Setup a test project with gtest dependencies
option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(vendor/googletest)
endif()

#find_package(glm REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
# Add header only spdlog directory
set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)

include(./cmake/imgui.cmake)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_glfw.cpp COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_glfw.h COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_vulkan.cpp ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_vulkan.cpp COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_vulkan.h ${CMAKE_CURRENT_SOURCE_DIR}/bindings/imgui_impl_vulkan.h COPYONLY)