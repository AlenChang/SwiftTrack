cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)

set(TARGET lib)

add_library(
        ${TARGET}
        my_movmedian_data.cpp my_movmedian_data.h
        my_movmedian_emxutil.cpp my_movmedian_emxutil.h
        my_movmedian_initialize.cpp my_movmedian_initialize.h
        my_movmedian_terminate.cpp my_movmedian_terminate.h
        my_movmedian_types.h
        my_movmedian.cpp my_movmedian.h
        quickselect.cpp quickselect.h
        rtwtypes.h tmwtypes.h
)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})