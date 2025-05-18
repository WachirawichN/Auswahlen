# Graphic source files
file(GLOB_RECURSE GRAPHIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.cpp)

# Simulation source file
set(SIMULATION_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/simulation/simulation.cpp)

# Application source file
set(APP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/app/app.cpp)

# Final library
add_library(AUSWAHLEN_LIB STATIC
    ${GRAPHIC_SOURCES}
    ${SIMULATION_SOURCE}
    ${APP_SOURCE}
)
target_include_directories(AUSWAHLEN_LIB PUBLIC
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(AUSWAHLEN_LIB PUBLIC
    GLFW
)