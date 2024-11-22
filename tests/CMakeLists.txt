add_executable(source_extractor_test ${CMAKE_CURRENT_SOURCE_DIR}/source_extractor_test.cpp)
target_link_libraries(source_extractor_test PUBLIC imsumlib_static)
target_compile_options(source_extractor_test PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(source_extractor_test PRIVATE ${COMMON_LINK_OPTIONS})

add_executable(delaney_test ${CMAKE_CURRENT_SOURCE_DIR}/delaney_test.cpp)
target_link_libraries(delaney_test PUBLIC imsumlib_static)
target_compile_options(delaney_test PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(delaney_test PRIVATE ${COMMON_LINK_OPTIONS})

add_executable(reduction_test ${CMAKE_CURRENT_SOURCE_DIR}/reduction_test.cpp)
target_link_libraries(reduction_test PUBLIC imsumlib_static)
target_compile_options(reduction_test PRIVATE ${COMMON_COMPILE_OPTIONS})
target_link_options(reduction_test PRIVATE ${COMMON_LINK_OPTIONS})
