cmake_minimum_required(VERSION 4.0.0)
project(Auswahlen VERSION 0.2.0 LANGUAGES CXX)

# Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(asset)
add_subdirectory(dependencies)
add_subdirectory(src)

# Executable
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)

#Library linking
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    AUSWAHLEN_LIB
)