cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)

set(TARGET lib)

add_library(
       ${TARGET}
       NoiseSupression.cpp
       NoiseSupression.h
       NoiseSupression_data.cpp
       NoiseSupression_data.h
       NoiseSupression_types.h
       quickselect.cpp
       quickselect.h
       rtwtypes.h
       tmwtypes.h

)
set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
