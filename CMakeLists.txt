cmake_minimum_required(VERSION 3.19)
project(INI_Parser VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(INI_Parser src/main.cpp common/INIMapImpl.h reader/INIReaderImpl.h)

target_include_directories(INI_Parser PRIVATE common reader)

