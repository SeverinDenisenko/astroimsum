set(IMSUM_LIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/fits.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sumation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source_extractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/astrometric_reduction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/delaney.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame_frocessing.cpp)
add_library(imsumlib OBJECT ${IMSUM_LIB_SRC})
set_property(TARGET imsumlib PROPERTY POSITION_INDEPENDENT_CODE 1)

target_compile_options(imsumlib PUBLIC ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL sep wcslib config++)
target_include_directories(imsumlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(imsumlib_shared SHARED $<TARGET_OBJECTS:imsumlib>)
add_library(imsumlib_static STATIC $<TARGET_OBJECTS:imsumlib>)

target_compile_options(imsumlib_shared PUBLIC ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib_shared PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib_shared PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL sep wcslib config++)
target_include_directories(imsumlib_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(imsumlib_static PUBLIC ${COMMON_COMPILE_OPTIONS})
target_link_options(imsumlib_static PUBLIC ${COMMON_LINK_OPTIONS})
target_link_libraries(imsumlib_static PUBLIC ${Boost_LIBRARIES} cfitsio triangle GSL sep wcslib config++)
target_include_directories(imsumlib_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
