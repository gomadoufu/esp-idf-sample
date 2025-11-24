
cmake_minimum_required(VERSION 3.16)

# cbor特有の警告を抑制
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-macro-redefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(arduino_mimic)
