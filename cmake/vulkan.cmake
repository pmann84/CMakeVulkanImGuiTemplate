set(VULKAN_VERSION 1.3.224.0)
set(VULKAN_INSTALL_PATH "C:\\VulkanSDK\\${VULKAN_VERSION}")
set(VULKAN_INCLUDE_DIRS "${VULKAN_INSTALL_PATH}\\Include")
set(VULKAN_LIB_DIRS "${VULKAN_INSTALL_PATH}\\Lib")
set(VULKAN_LIB_BIN "${VULKAN_INSTALL_PATH}\\Bin")
set(VULKAN_LIB "vulkan-1.lib")
set(VULKAN_GLSLC_EXE "${VULKAN_LIB_BIN}\\glslc.exe")

# Shader Compilation
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_command(
  COMMAND
    ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
  OUTPUT ${SHADER_BINARY_DIR}
  COMMENT "Creating ${SHADER_BINARY_DIR}"
)

# Find the shaders
file(GLOB SHADERS
  ${SHADER_SOURCE_DIR}/*.vert
  ${SHADER_SOURCE_DIR}/*.frag
#  ${SHADER_SOURCE_DIR}/*.comp
#  ${SHADER_SOURCE_DIR}/*.geom
#  ${SHADER_SOURCE_DIR}/*.tesc
#  ${SHADER_SOURCE_DIR}/*.tese
#  ${SHADER_SOURCE_DIR}/*.mesh
#  ${SHADER_SOURCE_DIR}/*.task
#  ${SHADER_SOURCE_DIR}/*.rgen
#  ${SHADER_SOURCE_DIR}/*.rchit
#  ${SHADER_SOURCE_DIR}/*.rmiss)
)

# Compile all shaders
message("Found shaders: ${SHADERS}")
foreach(source IN LISTS SHADERS)
    get_filename_component(FILENAME ${source} NAME)
    message("Compiling shader ${FILENAME}")
    add_custom_command(
        COMMAND
        ${glslc_executable} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv ${source} # -MD -MF ${SHADER_BINARY_DIR}/${FILENAME}.d
        OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
        DEPENDS ${source} ${SHADER_BINARY_DIR}
        COMMENT "Compiling ${FILENAME}"
  )
  list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach()

message("Shaders compiled to: ${SPV_SHADERS}")

add_custom_target(vulkan_shaders ALL DEPENDS ${SPV_SHADERS})