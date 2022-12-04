cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)
set(TARGET engine)

add_subdirectory(util)
add_subdirectory(recalibrateHistory)
add_subdirectory(fftw-3.3.10)
#add_subdirectory(generated)
# find_library( # Sets the name of the path variable.
#         log-lib

#         # Specifies the name of the NDK library that
#         # you want CMake to locate.
#         log )
add_library(
        ${TARGET}
        STATIC
        engine.cpp
        preprocessor.cpp
        denoiser.cpp
        postprocessor.cpp
)
#link_directories(C:/Users/v-yongzhang/Documents/SwiftTrack/app/src/main/cpp/engine/fftw-3.3.5-dll64)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} include fftw-3.3.10/api)

#target_link_libraries(${TARGET} util recalibrate)

target_link_libraries(${TARGET} util recalibrate fftw3)

