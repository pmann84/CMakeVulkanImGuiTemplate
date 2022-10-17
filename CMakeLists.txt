cmake_minimum_required(VERSION 3.22)

# Project settings
project(
        VulkanImGuiTemplate
        VERSION 1.0.0
        DESCRIPTION ""
        LANGUAGES CXX C
)

# CMake scripts
include(cmake/vulkan.cmake) # Vulkan paths and version
include(cmake/project_defaults.cmake) # Default project settings
include(cmake/project_dependencies.cmake) # Project dependencies

# App framework and app stub
add_subdirectory(framework)
add_subdirectory(app)