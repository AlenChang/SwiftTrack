cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)
set(TARGET recalibrate)

file(GLOB_RECURSE generated_srcs *.c *.h)

add_library(
    ${TARGET}
    ${generated_srcs}
)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

