cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(astroimsum VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/cfitsio-4.5.0)
add_subdirectory(deps/TRIANGLE)
add_subdirectory(deps/GSL)
find_package(Boost COMPONENTS thread REQUIRED)

set(IMSUM_LIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/fits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/frame_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/sumation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/source_extractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/astrometric_reduction.cpp)
add_library(imsumlib ${IMSUM_LIB_SRC})

target_compile_options(imsumlib PRIVATE -O3 -Wall -Wextra -pedantic)
target_link_options(imsumlib PRIVATE)
target_link_libraries(imsumlib PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL)
target_include_directories(imsumlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/include)

set(IMSUM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsum/main.cpp)
add_executable(imsum ${IMSUM_SRC})

target_compile_options(imsum PRIVATE -O3 -Wall -Wextra -pedantic)
target_link_options(imsum PRIVATE)
target_link_libraries(imsum PUBLIC imsumlib)

# tests

add_executable(source_extractor_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/source_extractor_test.cpp)
target_link_libraries(source_extractor_test PUBLIC imsumlib)
