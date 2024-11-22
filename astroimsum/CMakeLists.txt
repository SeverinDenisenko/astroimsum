set(IMSUM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(imsum ${IMSUM_SRC})

target_compile_options(imsum PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(imsum PRIVATE ${COMMON_LINK_OPTIONS})
target_link_libraries(imsum PUBLIC imsumlib_static)
