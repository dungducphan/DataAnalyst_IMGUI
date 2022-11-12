cmake_minimum_required(VERSION 3.23)
project(ROOTImGUI)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.2 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(imgui)
include_directories(imgui/backends)
include_directories(implot)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(CustomUI)

file(GLOB IMPLOT_SRC            implot/*.cpp)
file(GLOB IMPLOT_HEADER         implot/*.h)
file(GLOB IMGUI_SRC             imgui/*.cpp)
file(GLOB IMGUI_HEADER          imgui/*.h)
file(GLOB IMGUI_BACKEND_SRC     imgui/backends/imgui_impl_glfw.cpp
                                imgui/backends/imgui_impl_vulkan.cpp)
file(GLOB IMGUI_BACKEND_HEADER  imgui/backends/imgui_impl_glfw.h
                                imgui/backends/imgui_impl_vulkan.h)

file(GLOB CUSTOM_UI             CustomUI/*.*)

add_executable(ROOTImGUI main.cpp   ${IMGUI_SRC} ${IMGUI_HEADER}
                                    ${IMPLOT_SRC} ${IMPLOT_HEADER}
                                    ${IMGUI_BACKEND_SRC} ${IMGUI_BACKEND_HEADER}
                                    ${CUSTOM_UI})

target_link_libraries(ROOTImGUI glfw)

target_include_directories(ROOTImGUI PUBLIC ${VULKAN_INCLUDE_DIRS})
target_link_libraries(ROOTImGUI Vulkan::Vulkan)
