set(IMSUMLIB_C_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib.cpp)
add_library(imsumlib_c SHARED ${IMSUMLIB_C_SRC})

target_compile_options(imsumlib_c PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib_c PRIVATE ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib_c PRIVATE imsumlib_static)
target_include_directories(imsumlib_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
