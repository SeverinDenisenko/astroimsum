set(IMSUMLIB_C_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib.cpp)
add_library(astroimsumlib_c SHARED ${IMSUMLIB_C_SRC})

target_compile_options(astroimsumlib_c PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(astroimsumlib_c PRIVATE ${COMMON_LINK_OPTIONS})
target_link_libraries(astroimsumlib_c PRIVATE imsumlib_static)
target_include_directories(astroimsumlib_c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_command(TARGET astroimsumlib_c POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:astroimsumlib_c> ${CMAKE_CURRENT_SOURCE_DIR}/../pyastroimsumlib
)
