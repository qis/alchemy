#pragma once

// ============================================================================
// config
// ============================================================================

#define ALCHEMY_DEBUG 0
#define ALCHEMY_PRECISE 0

// ============================================================================
// system
// ============================================================================

#include <windows.h>
#include <stdio.h>

// ============================================================================
// skse64
// ============================================================================

#include <skse64_common/skse_version.h>

// ============================================================================
// common
// ============================================================================

#include <RE/Skyrim.h>
#include <SKSE/API.h>

// ============================================================================
// spdlog
// ============================================================================

#if ALCHEMY_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif

#include <spdlog/spdlog.h>

#define LOGT SPDLOG_TRACE
#define LOGD SPDLOG_DEBUG
#define LOGI SPDLOG_INFO
#define LOGW SPDLOG_WARN
#define LOGE SPDLOG_ERROR
#define LOGC SPDLOG_CRITICAL

#if ALCHEMY_DEBUG

#define LOG_PATTERN "[%T.%e] [%^%L%$] [%@] %v"

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#endif

// ============================================================================
// tbb
// ============================================================================

#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>

// ============================================================================
// standard
// ============================================================================

#include <algorithm>
#include <array>
#include <exception>
#include <execution>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <cstdio>
#include <cstdlib>
