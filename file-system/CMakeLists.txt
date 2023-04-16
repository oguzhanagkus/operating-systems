cmake_minimum_required(VERSION 3.22)
project(file_system)

set(CMAKE_CXX_STANDARD 14)

add_executable(makeFileSystem MakeFileSystem.cpp Helper.cpp Helper.h FileSystem.cpp FileSystem.h)
add_executable(fileSystemOper FileSystemOperation.cpp Helper.cpp Helper.h FileSystem.cpp FileSystem.h)
