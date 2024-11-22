cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(astroimsum VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/cfitsio-4.5.0)
add_subdirectory(deps/TRIANGLE)
add_subdirectory(deps/GSL)
find_package(Boost COMPONENTS thread REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_COMPILE_OPTIONS -O0 -g)
    set(COMMON_LINK_OPTIONS)
elseif(CMAKE_BUILD_TYPE STREQUAL "DebugASAN")
    set(COMMON_COMPILE_OPTIONS -O0 -g -fsanitize=address)
    set(COMMON_LINK_OPTIONS -fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "DebugTSAN")
    set(COMMON_COMPILE_OPTIONS -O0 -g -fsanitize=thread)
    set(COMMON_LINK_OPTIONS -fsanitize=thread)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(COMMON_COMPILE_OPTIONS -O3)
    set(COMMON_LINK_OPTIONS)
else()
    set(COMMON_COMPILE_OPTIONS -O3)
    set(COMMON_LINK_OPTIONS)
endif()

set(IMSUM_LIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/fits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/frame_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/sumation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/source_extractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/astrometric_reduction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/delaney.cpp)
add_library(imsumlib OBJECT ${IMSUM_LIB_SRC})
set_property(TARGET imsumlib PROPERTY POSITION_INDEPENDENT_CODE 1)

target_compile_options(imsumlib PUBLIC -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL)
target_include_directories(imsumlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/include)

add_library(imsumlib_shared SHARED $<TARGET_OBJECTS:imsumlib>)
add_library(imsumlib_static STATIC $<TARGET_OBJECTS:imsumlib>)

target_compile_options(imsumlib_shared PUBLIC -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib_shared PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib_shared PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL)
target_include_directories(imsumlib_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/include)

target_compile_options(imsumlib_static PUBLIC -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib_static PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib_static PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL)
target_include_directories(imsumlib_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/astroimsumlib/include)

set(IMSUM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/astroimsum/main.cpp)
add_executable(imsum ${IMSUM_SRC})

target_compile_options(imsum PRIVATE -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(imsum PRIVATE ${COMMON_LINK_OPTIONS})
target_link_libraries(imsum PUBLIC imsumlib_static)

# tests

add_executable(source_extractor_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/source_extractor_test.cpp)
target_link_libraries(source_extractor_test PUBLIC imsumlib_static)
target_compile_options(source_extractor_test PRIVATE -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(source_extractor_test PRIVATE ${COMMON_LINK_OPTIONS})

add_executable(delaney_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/delaney_test.cpp)
target_link_libraries(delaney_test PUBLIC imsumlib_static)
target_compile_options(delaney_test PRIVATE -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(delaney_test PRIVATE ${COMMON_LINK_OPTIONS})

add_executable(reduction_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/reduction_test.cpp)
target_link_libraries(reduction_test PUBLIC imsumlib_static)
target_compile_options(reduction_test PRIVATE -Wall -Wextra -pedantic ${COMMON_COMPILE_OPTIONS})
target_link_options(reduction_test PRIVATE ${COMMON_LINK_OPTIONS})
