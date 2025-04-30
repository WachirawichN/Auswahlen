cmake_minimum_required(VERSION 4.0.0)
project(Auswahlen VERSION 0.1.1 LANGUAGES C CXX CUDA)

find_package(CUDAToolkit REQUIRED)
enable_language(CUDA)

# Enable separable compilation and device linking
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

# Set standard and CUDA flags
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CUDA_SEPARABLE_COMPILATION ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -rdc=true")
set(CMAKE_CUDA_ARCHITECTURES 86)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(dependencies)
add_subdirectory(src)

# Executable
add_executable(Auswahlen src/main.cpp)
# Required for device linking
set_target_properties(Auswahlen PROPERTIES
    LINKER_LANGUAGE CUDA
    CUDA_SEPARABLE_COMPILATION ON
)
target_include_directories(Auswahlen PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/include
)
target_link_libraries(Auswahlen PRIVATE
    GLAD
    ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib
    opengl32.lib
    stb
    GLC
    AuswahlenLib
)

# Copy shader files
add_custom_command(
    TARGET Auswahlen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/asset
            $<TARGET_FILE_DIR:Auswahlen>/asset
    COMMENT "Copying shader files"
)