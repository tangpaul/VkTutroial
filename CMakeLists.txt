cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(VkTutorial)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置脚本文件的路径
get_filename_component(PARENT_DIR ${CMAKE_BINARY_DIR} DIRECTORY)
message(STATUS "Binary directory: ${PARENT_DIR}")
set(SCRIPT_PATH "${PARENT_DIR}/shaders/compiler.cmd")

# 添加一个自定义目标来运行脚本
add_custom_target(RunScriptBeforeBuild ALL
  COMMAND cmd /c ${SCRIPT_PATH}
  COMMENT "Running ${SCRIPT_PATH} before building the project..."
)

# Find Vulkan
find_package(Vulkan REQUIRED)

# vendor
set(VENDOR_DIR vendor)
# eigen
set(EIGEN_SOURCE_DIR ${VENDOR_DIR}/Eigen)
# glm
set(GLM_SOURCE_DIR ${VENDOR_DIR}/glm)
# imgui
set(IMGUI_SOURCE_DIR ${VENDOR_DIR}/imgui)
file(GLOB IMGUI_SOURCE_FILE  "${IMGUI_SOURCE_DIR}/*.cpp")

#glfw
set(GLFW_DIR ${VENDOR_DIR}/GLFW)
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
add_subdirectory(${GLFW_DIR} GLFW_BUILD)

# src
set(SRC_DIR src)
file(GLOB SRC_FILE  "${SRC_DIR}/*.cpp")

# 添加你的源代码文件
add_executable(VkTutorial
                ${SRC_FILE}
                ${IMGUI_SOURCE_FILE}
                ${STB_SOURCE_FILE}
                )
target_include_directories(VkTutorial PRIVATE
                            ${SRC_DIR}
                            ${EIGEN_SOURCE_DIR}
                            ${GLFW_INCLUDE_DIR}
                            ${VENDOR_DIR}
                            ${GLM_SOURCE_DIR}
                            ${IMGUI_SOURCE_DIR}
                            ${Vulkan_INCLUDE_DIRS}
                            )
add_dependencies(VkTutorial RunScriptBeforeBuild)
# 链接库
if(WIN32)
    set(OTHER_LIBRARIES dwmapi)
endif()
target_link_libraries(VkTutorial glfw ${Vulkan_LIBRARIES} ${OTHER_LIBRARIES})