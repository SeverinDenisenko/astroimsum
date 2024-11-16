cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(astroimsum VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(deps/cfitsio-4.5.0)
add_subdirectory(deps/TRIANGLE)
find_package(Boost COMPONENTS thread REQUIRED)

set(IMSUM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/frame_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sumation.cpp)
add_executable(imsum ${IMSUM_SRC})
target_compile_options(imsum PRIVATE -O3 -Wall -Wextra -pedantic )
target_link_options(imsum PRIVATE )
target_link_libraries(imsum PUBLIC ${Boost_LIBRARIES} cfitsio triangle)
