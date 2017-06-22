cmake_minimum_required(VERSION 3.7)
project(miniSQL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp BufferManager.cpp BufferManager.h BufferUnit.cpp BufferUnit.h RecordManager.cpp
        RecordManager.h Basis.h TableStruct.h API.cpp API.h Catalog.cpp Catalog.h Interpreter.cpp Interpreter.h)
add_executable(miniSQL ${SOURCE_FILES})