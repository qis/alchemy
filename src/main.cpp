#include "alchemy.hpp"
#include "version.h"

extern "C" __declspec(dllexport) bool SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info);
extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface* skse);

bool SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info) {
  SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\" PROJECT_NAME ".log");
  SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
  SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
  SKSE::Logger::UseLogStamp(true);
  LOG_MESSAGE("%s v%s query", PROJECT_NAME, PROJECT_VERSION);
  info->infoVersion = SKSE::PluginInfo::kVersion;
  info->name = PROJECT_NAME;
  info->version = PROJECT_VERSION_MAJOR;
  if (skse->IsEditor()) {
    LOG_FATALERROR("loaded in editor, marking as incompatible");
    return false;
  }
  switch (skse->RuntimeVersion()) {
  case RUNTIME_VERSION_1_5_97:
    break;
  default:
    LOG_FATALERROR("unsupported runtime version: %08X", skse->RuntimeVersion());
    return false;
  }
  return true;
}

bool SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
  LOG_MESSAGE("%s v%s load", PROJECT_NAME, PROJECT_VERSION);
  if (!SKSE::Init(skse)) {
    return false;
  }

#if ALCHEMY_DEBUG
  AllocConsole();
  freopen("CONOUT$", "w", stdout);
  spdlog::init_thread_pool(8192, 1);
  spdlog::stdout_color_mt<spdlog::async_factory>("console")->set_pattern(LOG_PATTERN);
  spdlog::set_default_logger(spdlog::get("console"));
#endif

  try {
    alchemy::Load();
  }
  catch (const std::exception & e) {
    LOG_FATALERROR("exception: %s", e.what());
  }
  catch (...) {
    LOG_FATALERROR("exception");
  }
  return true;
}
