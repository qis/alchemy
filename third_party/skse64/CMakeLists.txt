cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(skse64 VERSION 2.0.17 LANGUAGES CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
  common/*.h
  common/*.cpp
  skse64/skse64/*.h
  skse64/skse64/*.cpp
  skse64/skse64_common/*.h
  skse64/skse64_common/*.cpp
  skse64/xbyak/*.h)

add_library(skse64 STATIC ${sources})

target_include_directories(skse64 PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/skse64)

target_compile_definitions(skse64 PRIVATE RUNTIME RUNTIME_VERSION=0x01050610)

target_compile_options(skse64 PUBLIC
  "/FI${CMAKE_CURRENT_SOURCE_DIR}/common/IPrefix.h"
  "/FI${CMAKE_CURRENT_SOURCE_DIR}/common/IDebugLog.h")

add_library(skse64::skse64 ALIAS skse64)

