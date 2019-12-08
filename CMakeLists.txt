cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(alchemy VERSION 0.1.0 LANGUAGES CXX)

configure_file(res/version.h.in ${CMAKE_BINARY_DIR}/src/version.h LF)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.hpp src/*.cpp)

add_library(alchemy SHARED ${sources})
target_include_directories(alchemy PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src)
target_include_directories(alchemy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(alchemy PRIVATE src/main.hpp)

add_subdirectory(third_party/skse64)
target_link_libraries(alchemy PRIVATE skse64::skse64)

add_subdirectory(third_party/common)
target_link_libraries(alchemy PRIVATE common::common)

add_subdirectory(third_party/spdlog)
target_link_libraries(alchemy PRIVATE spdlog::spdlog)

add_subdirectory(third_party/tbb)
target_link_libraries(alchemy PRIVATE TBB::tbb TBB::malloc)

install(TARGETS alchemy RUNTIME DESTINATION SKSE/Plugins)
