cmake_minimum_required (VERSION 3.1)
project(audio_out_lib)

# Prepare app env

set(APP_NAME audio_out)
set(audio_out_lib_VERSION_MAJOR 0)
set(audio_out_lib_VERSION_MINOR 41)

# Prepare compiler flags

if (WIN32)
	set (CMAKE_CXX_STANDARD 14)
else()
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall -Wextra -std=c++14")
  set(CMAKE_CXX_FLAGS_DEBUG
	 "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -DDEBUG -O0")
  set(CMAKE_CXX_FLAGS_RELEASE
	 "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

# Choose build type

if ("${CMAKE_BUILD_TYPE}" STREQUAL Release)
  set(CMAKE_BUILD_TYPE "Release")
else()
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Building ${CMAKE_BUILD_TYPE}")

# Install

set(SRC_FILES audio_out.h audio_out.cc audio_fx.h audio_fx.cc)
add_library(audio STATIC ${SRC_FILES})
install(TARGETS audio DESTINATION .)