cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(common VERSION 0.1.0 LANGUAGES CXX)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS include/*.h src/*.cpp)

add_library(common STATIC ${sources})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(common PUBLIC skse64::skse64)

target_compile_options(common PUBLIC "/FI${CMAKE_CURRENT_SOURCE_DIR}/include/ForceInclude.h")
target_compile_options(common PUBLIC "/FI${CMAKE_CURRENT_SOURCE_DIR}/include/SKSE/Logger.h")

add_library(common::common ALIAS common)
