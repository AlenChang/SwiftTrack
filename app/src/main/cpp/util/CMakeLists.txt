cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)

set(TARGET util)

add_library(
        ${TARGET}
        filter_util.hpp matrix_util.hpp
)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
