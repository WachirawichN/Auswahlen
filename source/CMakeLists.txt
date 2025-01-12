add_subdirectory(dependencies)
add_subdirectory(graphic)
add_subdirectory(simulation)


set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/source/dependencies/GLEW/lib/Release/x64")
set(GLEW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/source/dependencies/GLEW/include/GL")
add_library(glew32s STATIC IMPORTED)
set_target_properties(glew32s PROPERTIES
    IMPORTED_LOCATION "${GLEW_LIBRARIES}/glew32s.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_PATH}"
)

set(GLFW_LIBRARIES "${CMAKE_SOURCE_DIR}/source/dependencies/GLFW")
set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/source/dependencies/GLFW")
add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${GLFW_LIBRARIES}/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIRS}"
)



add_executable(Auswahlen main.cpp)

find_package(OpenGL REQUIRED)
target_link_libraries(Auswahlen PUBLIC OpenGL::GL)

target_include_directories(Auswahlen PUBLIC "${GLEW_INCLUDE_DIRS}")
target_link_libraries(Auswahlen PUBLIC glew32s)

target_include_directories(Auswahlen PUBLIC "${GLFW_INCLUDE_DIRS}")
target_link_libraries(Auswahlen PUBLIC glfw3)

target_link_libraries(Auswahlen PUBLIC glm)
target_link_libraries(Auswahlen PUBLIC stb_image)

target_link_libraries(Auswahlen PUBLIC simulation)
target_link_libraries(Auswahlen PUBLIC physic)
target_link_libraries(Auswahlen PUBLIC object)
target_link_libraries(Auswahlen PUBLIC mathExt)

target_link_libraries(Auswahlen PUBLIC graphic)
target_link_libraries(Auswahlen PUBLIC geometry)


# Copy shader file into build directory
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/source/graphic/res/shader")
set(SHADER_FILE "${SHADER_DIR}/3d.shader")
set(SHADER_OUT_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../graphic/res/3d.shader")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_FILE} ${SHADER_OUT_DIR}
)