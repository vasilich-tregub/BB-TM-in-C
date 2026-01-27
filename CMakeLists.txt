# TERMS OF USE
# This source code is subject to the terms of the MIT License. 
# Copyright(c) 2026 Vladimir Vasilich Tregub
#
# CMakeList.txt : CMake project for BB-TM-in-C, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("BB-TM-in-C")

# Add source to this project's executable.
add_executable (BB-TM-in-C "BB-TM-in-C.c" "BB-TM-in-C.h")
add_executable (webui-BBTM "webui-BBTM.c" "cc-BB.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BB-TM-in-C PROPERTY C_STANDARD 11)
  set_property(TARGET webui-BBTM PROPERTY C_STANDARD 11)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/favicon128.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/index.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cc-bb.js DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wa-bb/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/wa-bb/)

# TODO: Add tests and install targets if needed.
