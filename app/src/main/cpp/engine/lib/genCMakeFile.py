import os

path = "./"
dirs = os.listdir(path)

with open("CMakeLists.txt", "w") as f:
    f.write("cmake_minimum_required(VERSION 3.10.2)\r")
    f.write("\r")
    f.write("project(acoustic)\r")
    f.write("\r")
    f.write("set(CMAKE_CXX_STANDARD 14)\r")
    f.write("\r")
    f.write("set(TARGET lib)\r")
    f.write("\r")
    f.write("add_library(\r")
    f.write("       ${TARGET}\r")
    for i in dirs:
        if os.path.splitext(i)[1] == ".h" or os.path.splitext(i)[1] == ".cpp":
            f.write("       " + i+"\r")
    f.write("\r")
    f.write(")\r")
    f.write("set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)\r")
    f.write("\r")
    f.write("target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})\r")

# print(dirs)