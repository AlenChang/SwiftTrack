cmake_minimum_required(VERSION 3.10.2)

project(acoustic)

set(CMAKE_CXX_STANDARD 14)

set(TARGET generated)

#set(CMAKE_INSTALL_PREFIX C:/Users/reyna/Documents/HMC/Clinic/FFTW_Install)
set(FFTW3_DIR ../fftw-3.3.10)
add_subdirectory(${FFTW3_DIR} ${CMAKE_CURRENT_BINARY_DIR}/fftw_build)
include_directories(/usr/local/Cellar/fftw/3.3.10/include/)
#target_link_libraries (native-lib fftw3 AudioEngine log oboe android)

file(GLOB_RECURSE generated_srcs *.cpp detrend_alg.cpp)

add_library(
    ${TARGET}
    ${generated_srcs}
#        processFramePipe.cpp
)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} fftw3)
