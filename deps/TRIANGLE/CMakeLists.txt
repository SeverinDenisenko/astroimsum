add_library(triangle STATIC triangle.c)
target_include_directories(triangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(COMMON_COMPILE_OPTIONS -O3 -Wno-deprecated-non-prototype)
set(COMMON_LINK_OPTIONS )

target_compile_options(triangle PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(triangle PRIVATE ${COMMON_LINK_OPTIONS})
