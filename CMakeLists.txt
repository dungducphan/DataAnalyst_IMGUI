cmake_minimum_required(VERSION 3.23)
project(ROOTImGUI)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(ROOT REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(imgui)
include_directories(imgui/backends)
include_directories(implot)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(CustomUI)
include_directories(CustomUI/ImGuiFileDialog)
include(${ROOT_USE_FILE})

file(GLOB IMPLOT_SRC            implot/*.cpp)
file(GLOB IMPLOT_HEADER         implot/*.h)
file(GLOB IMGUI_SRC             imgui/*.cpp)
file(GLOB IMGUI_HEADER          imgui/*.h)
file(GLOB IMGUI_BACKEND_SRC     imgui/backends/imgui_impl_glfw.cpp
                                imgui/backends/imgui_impl_vulkan.cpp)
file(GLOB IMGUI_BACKEND_HEADER  imgui/backends/imgui_impl_glfw.h
                                imgui/backends/imgui_impl_vulkan.h)

file(GLOB CUSTOM_UI             CustomUI/*.*)
file(GLOB IMFILEDIALOG_SRC      CustomUI/ImGuiFileDialog/*.cpp)
file(GLOB IMFILEDIALOG_HEADER   CustomUI/ImGuiFileDialog/*.h)

add_executable(ROOTImGUI main.cpp   ${IMGUI_SRC} ${IMGUI_HEADER}
                                    ${IMPLOT_SRC} ${IMPLOT_HEADER}
                                    ${IMGUI_BACKEND_SRC} ${IMGUI_BACKEND_HEADER}
                                    ${CUSTOM_UI}
                                    ${IMFILEDIALOG_SRC} ${IMFILEDIALOG_HEADER})

target_link_libraries(ROOTImGUI glfw)

target_include_directories(ROOTImGUI PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(ROOTImGUI Vulkan::Vulkan)

target_link_libraries(ROOTImGUI ${ROOT_LIBRARIES})