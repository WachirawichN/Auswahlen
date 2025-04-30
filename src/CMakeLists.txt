# Auswahlen's library
file(GLOB_RECURSE AUSWAHLEN_LIB_SRC_CU ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cu)
file(GLOB_RECURSE AUSWAHLEN_LIB_SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)

add_library(AuswahlenLib STATIC ${AUSWAHLEN_LIB_SRC_CU} ${AUSWAHLEN_LIB_SRC_CPP})
set_property(TARGET AuswahlenLib PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)

target_include_directories(AuswahlenLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(AuswahlenLib PUBLIC GLC)
target_link_libraries(AuswahlenLib PUBLIC GLAD)
target_link_libraries(AuswahlenLib PUBLIC stb)

set_target_properties(AuswahlenLib PROPERTIES
    LINKER_LANGUAGE CUDA
    CUDA_SEPARABLE_COMPILATION ON
)