cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

add_subdirectory(util)
add_subdirectory(lib)
add_subdirectory(engine)

add_library(acoustic SHARED audio_processor.cpp)

find_library(log-lib log)

target_link_libraries(acoustic ${log-lib} util engine lib)