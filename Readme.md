# Prerequisites
- Vulkan SDK (download and install from [here](https://vulkan.lunarg.com/sdk/home))
- CMake
- spdlog
- glm
- glad
- glfw
- ImGui

# Clone
For a new clone
```
git clone --recursive https://github.com/pmann84/CMakeVulkanImGuiTemplate.git
```
For an already cloned repo
```
git clone https://github.com/pmann84/CMakeVulkanImGuiTemplate.git
cd CMakeVulkanImGuiTemplate
git submodule update --init --recursive
```
# Configure and Build
First do an out of source build by configuring the build scripts
```
cd CMakeVulkanImGuiTemplate
mkdir out
cd out
cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON ..
```
Then build
```
cd out 
cmake --build . --config Debug
```