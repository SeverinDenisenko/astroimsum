add_library(triangle STATIC triangle.c)
target_include_directories(triangle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
