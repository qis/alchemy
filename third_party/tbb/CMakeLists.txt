cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_MSVC_RUNTIME_LIBRARY "")
project(tbb VERSION 2019.9 LANGUAGES CXX ASM_MASM)

# Version
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/include/version_string.ver
  "#define __TBB_VERSION_STRINGS(N) \"${PROJECT_VERSION}\"")

# Headers
file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/tbb/*.h)

# TBB::tbb
file(GLOB_RECURSE tbb_headers CONFIGURE_DEPENDS src/tbb/*.h)

set(tbb_sources
  src/old/concurrent_queue_v2.cpp
  src/old/concurrent_vector_v2.cpp
  src/old/spin_rw_mutex_v2.cpp
  src/old/task_v2.cpp
  src/rml/client/rml_tbb.cpp
  src/tbb/arena.cpp
  src/tbb/cache_aligned_allocator.cpp
  src/tbb/concurrent_hash_map.cpp
  src/tbb/concurrent_monitor.cpp
  src/tbb/concurrent_queue.cpp
  src/tbb/concurrent_vector.cpp
  src/tbb/condition_variable.cpp
  src/tbb/critical_section.cpp
  src/tbb/dynamic_link.cpp
  src/tbb/governor.cpp
  src/tbb/intel64-masm/atomic_support.asm
  src/tbb/intel64-masm/intel64_misc.asm
  src/tbb/intel64-masm/itsx.asm
  src/tbb/itt_notify.cpp
  src/tbb/market.cpp
  src/tbb/mutex.cpp
  src/tbb/observer_proxy.cpp
  src/tbb/pipeline.cpp
  src/tbb/private_server.cpp
  src/tbb/queuing_mutex.cpp
  src/tbb/queuing_rw_mutex.cpp
  src/tbb/reader_writer_lock.cpp
  src/tbb/recursive_mutex.cpp
  src/tbb/scheduler.cpp
  src/tbb/semaphore.cpp
  src/tbb/spin_mutex.cpp
  src/tbb/spin_rw_mutex.cpp
  src/tbb/task_group_context.cpp
  src/tbb/task.cpp
  src/tbb/tbb_main.cpp
  src/tbb/tbb_misc_ex.cpp
  src/tbb/tbb_misc.cpp
  src/tbb/tbb_statistics.cpp
  src/tbb/tbb_thread.cpp
  src/tbb/x86_rtm_rw_mutex.cpp)

add_library(tbb STATIC ${headers} ${tbb_headers} ${tbb_sources})
target_include_directories(tbb PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(tbb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(tbb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(tbb PRIVATE DO_ITT_NOTIFY USE_WINTHREAD IN_CILK_STATIC __TBB_BUILD=1)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties(tbb PROPERTIES OUTPUT_NAME "tbb_debug")
  target_compile_definitions(tbb PRIVATE __TBB_LIB_NAME=tbb_debug.lib)
else()
  set_target_properties(tbb PROPERTIES OUTPUT_NAME "tbb")
  target_compile_definitions(tbb PRIVATE __TBB_LIB_NAME=tbb.lib)
endif()

option(TBB_USE_EXCEPTIONS "Use exceptions" ON)
if(NOT TBB_USE_EXCEPTIONS)
  target_compile_definitions(tbb PUBLIC TBB_USE_EXCEPTIONS=0)
endif()

# TBB::malloc
file(GLOB_RECURSE malloc_headers CONFIGURE_DEPENDS src/tbbmalloc/*.h)

set(malloc_sources
  src/tbb/intel64-masm/atomic_support.asm
  src/tbb/itt_notify.cpp
  src/tbbmalloc/backend.cpp
  src/tbbmalloc/backref.cpp
  src/tbbmalloc/frontend.cpp
  src/tbbmalloc/large_objects.cpp
  src/tbbmalloc/tbbmalloc.cpp)

add_library(malloc STATIC ${headers} ${malloc_headers} ${malloc_sources})
target_include_directories(malloc PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(malloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(malloc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(malloc PRIVATE DO_ITT_NOTIFY USE_WINTHREAD IN_CILK_STATIC __TBBMALLOC_BUILD=1)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties(malloc PROPERTIES OUTPUT_NAME "tbbmalloc_debug")
  target_compile_definitions(malloc PRIVATE __TBB_LIB_NAME=tbb_debug.lib)
else()
  set_target_properties(malloc PROPERTIES OUTPUT_NAME "tbbmalloc")
  target_compile_definitions(malloc PRIVATE __TBB_LIB_NAME=tbb.lib)
endif()

# Alias
add_library(TBB::tbb ALIAS tbb)
add_library(TBB::malloc ALIAS malloc)
