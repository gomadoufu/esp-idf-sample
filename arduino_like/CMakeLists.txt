
cmake_minimum_required(VERSION 3.16)

# cbor特有の警告を抑制
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-builtin-macro-redefined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-builtin-macro-redefined")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(arduino_like)

# https://docs.espressif.com/projects/espressif-ide/en/latest/additionalfeatures/clangd_cdt_support.html#how-to-fix-unknown-argument-error-when-navigating-to-the-esp-idf-components
if(EXISTS "${CMAKE_SOURCE_DIR}/fix_compile_commands.py")
  add_custom_target(
      fix_clangd ALL
      COMMAND ${CMAKE_COMMAND} -E echo "Running fix_compile_commands.py..."
      COMMAND ${CMAKE_COMMAND} -E env python3 ${CMAKE_SOURCE_DIR}/fix_compile_commands.py
      COMMENT "Cleaning compile_commands.json for clangd"
      VERBATIM
  )
endif()
