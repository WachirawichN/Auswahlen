# Get the executable's output directory.
if(CMAKE_CONFIGURATION_TYPES)  # Multi-config generators like Visual Studio.
    set(EXECUTABLE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR})
else()  # Single-config generators like Ninja or Unix Makefiles.
    set(EXECUTABLE_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    if(NOT EXECUTABLE_OUTPUT_DIR)
        set(EXECUTABLE_OUTPUT_DIR ${CMAKE_BINARY_DIR})
    endif()
endif()
message(STATUS "Asset directory: ${EXECUTABLE_OUTPUT_DIR}")

# Define asset, shader directory.
set(ASSET_MAKE_DIR ${EXECUTABLE_OUTPUT_DIR}/asset)
set(SHADER_MAKE_DIR ${ASSET_MAKE_DIR}/shader)

# Shader compilation and copy to shader directory.
file (GLOB SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shader/*.*)

# Compile all shader files
foreach(SHADER ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV_FILE ${SHADER_MAKE_DIR}/${FILE_NAME}.spv)

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND glslc ${SHADER} -o ${SPIRV_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader files: ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach()

# Add custom build target / only compile if shader file is changed.
add_custom_target(SHADERS ALL DEPENDS ${SPIRV_BINARY_FILES})