cmake_minimum_required(VERSION 3.8)
project(VulkanPathTracer VERSION 0.1.0)
add_executable(VulkanPathTracer 
    src/main.cpp
    src/App.cpp
    src/App.hpp
    src/VPTPipeline.cpp
    src/VPTPipeline.hpp
    src/VPTWindow.cpp
    src/VPTWindow.hpp
    src/VPTDevice.cpp
    src/VPTDevice.hpp
    src/VPTSwapChain.cpp
    src/VPTSwapChain.hpp
    src/VPTModel.cpp
    src/VPTModel.hpp
)
LINK_DIRECTORIES(
    C:/vcpkg/installed/x64-windows/lib
)
find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
target_include_directories(${PROJECT_NAME}  
    PUBLIC
        ${Vulkan_INCLUDE_DIRS}
        C:/vcpkg/installed/x64-windows/include
)
TARGET_LINK_LIBRARIES(VulkanPathTracer
    PRIVATE 
        glfw 
        glm::glm 
        Vulkan::Vulkan
)
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} 
            -DTARGET_NAME=${PROJECT_NAME}
            -DSHADER_PATH=${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
            -P ${CMAKE_SOURCE_DIR}/compileShaders.cmake
    COMMENT "Compiling Shaders..."
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/shaders/compiled
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)        
