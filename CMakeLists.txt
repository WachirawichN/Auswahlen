cmake_minimum_required(VERSION 4.0.0)
project(Auswahlen VERSION 0.2.0 LANGUAGES CXX)

# Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vulkan
find_package(Vulkan REQUIRED)

add_subdirectory(dependencies)

# Executable
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)

#Library linking
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/include
    $ENV{VULKAN_SDK}/include
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GLFW
    Vulkan::Vulkan
)