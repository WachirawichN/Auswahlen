cmake_minimum_required(VERSION 4.0.0)
project(Auswahlen VERSION 0.1.1 LANGUAGES C CXX CUDA)

# Enable separable compilation and device linking
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

# Set standard and CUDA flags
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 86)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -rdc=true")

# Libraries
# GLC library
file(GLOB_RECURSE GLC_SOURCE
    ${CMAKE_SOURCE_DIR}/dependencies/src/GLC/vector/*.cu
    ${CMAKE_SOURCE_DIR}/dependencies/src/GLC/matrix/*.cu
    ${CMAKE_SOURCE_DIR}/dependencies/src/GLC/utility/*.cu
)
add_library(GLC STATIC ${GLC_SOURCE})
# GLAD
add_library(GLAD STATIC ${CMAKE_SOURCE_DIR}/dependencies/src/glad/glad.c)
# stb
add_library(stb STATIC ${CMAKE_SOURCE_DIR}/dependencies/src/stb/stb_image.cpp)
# Auswahlen's library
file(GLOB_RECURSE AUSWAHLEN_LIB_SOURCE
    ${CMAKE_SOURCE_DIR}/src/*/*.cu
    ${CMAKE_SOURCE_DIR}/src/*/*.cpp
)
add_library(AuswahlenLib STATIC ${AUSWAHLEN_LIB_SOURCE})

# Executable
# Force TestCPU to be treated as a CUDA target
set_source_files_properties(main.cpp PROPERTIES LANGUAGE CUDA)
add_executable(Auswahlen src/main.cpp)
# Required for device linking
set_target_properties(Auswahlen
    PROPERTIES
    LINKER_LANGUAGE CUDA
    CUDA_SEPARABLE_COMPILATION ON
)
# Including directories and linking libraries
target_include_directories(Auswahlen PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(Auswahlen PRIVATE
    GLC
    GLAD
    ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib
    stb
    AuswahlenLib
    opengl32.lib
)

# Copy shader files
add_custom_command(
    TARGET Auswahlen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/asset
            $<TARGET_FILE_DIR:Auswahlen>/asset
    COMMENT "Copying shader files"
)