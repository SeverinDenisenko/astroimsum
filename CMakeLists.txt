cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(astroimsum VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/cfitsio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/TRIANGLE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/GSL)
find_package(Boost COMPONENTS thread REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -O0 -g)
    set(COMMON_LINK_OPTIONS)
elseif(CMAKE_BUILD_TYPE STREQUAL "DebugASAN")
    set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -O0 -g -fsanitize=address)
    set(COMMON_LINK_OPTIONS -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "DebugTSAN")
    set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -O0 -g -fsanitize=thread)
    set(COMMON_LINK_OPTIONS -fsanitize=thread)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -O3)
    set(COMMON_LINK_OPTIONS)
else()
    set(COMMON_COMPILE_OPTIONS -Wall -Wextra -pedantic -O3)
    set(COMMON_LINK_OPTIONS)
endif()

add_subdirectory(astroimsumlib)

add_subdirectory(astroimsumlib_c)

add_subdirectory(astroimsum)

add_subdirectory(tests)
