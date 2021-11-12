cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)
set(TARGET engine)

add_library(
        ${TARGET}
        engine.h engine.cpp
        preprocessor.h preprocessor.cpp
        denoiser.h denoiser.cpp
        postprocessor.h postprocessor.cpp
        histories.hpp
)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} util lib)
