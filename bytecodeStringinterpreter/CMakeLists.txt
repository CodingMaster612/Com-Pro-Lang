cmake_minimum_required(VERSION 3.17)
project(bytecodestringinterpreter)

set(CMAKE_CXX_STANDARD 20)

add_library(bytecodestringinterpreter_internals
        StringInstruction.cpp
        StringInstruction.hpp
        BytecodeString.cpp
        BytecodeString.hpp)

add_executable(bytecodeString
        main.cpp)

target_link_libraries(bytecodeStringinterperter bytecodeStringinterpreter_internals)