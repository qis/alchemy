#include <ShlObj.h>
#include "skse64_common/skse_version.h"
#include "skse64_common/Utilities.h"
#include "skse64_loader_common/LoaderError.h"
#include "skse64_loader_common/IdentifyEXE.h"
#include "skse64_loader_common/Steam.h"
#include "skse64_loader_common/Inject.h"
#include <string>
#include "common/IFileStream.h"
#include <tlhelp32.h>
#include "Options.h"

IDebugLog gLog;

static void PrintModuleInfo(UInt32 procID);
static void PrintProcessInfo();

int main(int argc, char ** argv)
{
	gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\skse64_loader.log");
	gLog.SetPrintLevel(IDebugLog::kLevel_FatalError);
	gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

	FILETIME	now;
	GetSystemTimeAsFileTime(&now);

	SKSE_MESSAGE("skse64 loader %08X %08X%08X %s",
		PACKED_SKSE_VERSION, now.dwHighDateTime, now.dwLowDateTime, GetOSInfoStr().c_str());

	if(!g_options.Read(argc, argv))
	{
		PrintLoaderError("Couldn't read arguments.");
		g_options.PrintUsage();

		return 1;
	}

	if(g_options.m_optionsOnly)
	{
		g_options.PrintUsage();
		return 0;
	}

	if(g_options.m_verbose)
		gLog.SetPrintLevel(IDebugLog::kLevel_VerboseMessage);

	if(g_options.m_launchCS)
	{
		PrintLoaderError("The editor should be launched directly.");

		return 1;
	}

	// get process/dll names
	bool		dllHasFullPath = false;
	const char	* baseDllName = g_options.m_launchCS ? "skse64_editor" : "skse64";
	bool		usedCustomRuntimeName = false;

	std::string	procName;

	if(g_options.m_launchCS)
	{
		procName = "CreationKit.exe";
	}
	else
	{
		procName = GetConfigOption("Loader", "RuntimeName");
		if(!procName.empty())
		{
			SKSE_MESSAGE("using runtime name from config: %s", procName.c_str());
			usedCustomRuntimeName = true;
		}
		else
		{
			procName = "SkyrimSE.exe";

			// simple check to see if someone kludge-patched the EXE
			// don't kludge the EXE, use the .ini file RIGHT ABOVE HERE
			UInt32	procNameCheck =
				(procName[0] <<  8) |
				(procName[1] << 24) |
				(procName[2] << 16) |
				(procName[3] <<  0);

			if(procNameCheck != 'kySr')
			{
				SKSE_ERROR("### someone kludged the default process name to (%s), don't ask me for support with your install ###", procName.c_str());
			}

			// check to see if someone screwed up their install
			std::string appName = GetRuntimeName();
			if(!_stricmp(appName.c_str(), procName.c_str()))
			{
				PrintLoaderError("You have renamed skse64_loader and have not specified the name of the runtime.");

				return 1;
			}
		}
	}

	const std::string & runtimeDir = GetRuntimeDirectory();
	std::string procPath = runtimeDir + "\\" + procName;

	if(g_options.m_altEXE.size())
	{
		procPath = g_options.m_altEXE;
		SKSE_MESSAGE("launching alternate exe (%s)", procPath.c_str());
	}

	SKSE_MESSAGE("procPath = %s", procPath.c_str());

	// check if the exe exists
	{
		IFileStream	fileCheck;
		if(!fileCheck.Open(procPath.c_str()))
		{
			if(usedCustomRuntimeName)
			{
				// hurr durr
				PrintLoaderError("Couldn't find %s. You have customized the runtime name via SKSE64's .ini file, and that file does not exist. This can usually be fixed by removing the RuntimeName line from the .ini file.)", procName.c_str());
			}
			else
			{
				PrintLoaderError("Couldn't find %s.", procName.c_str());
			}

			return 1;
		}
	}

	SKSE_MESSAGE("launching: %s (%s)", procName.c_str(), procPath.c_str());

	if(g_options.m_altDLL.size())
	{
		baseDllName = g_options.m_altDLL.c_str();
		SKSE_MESSAGE("launching alternate dll (%s)", baseDllName);

		dllHasFullPath = true;
	}

	std::string		dllSuffix;
	ProcHookInfo	procHookInfo;

	// check exe version
	if(!IdentifyEXE(procPath.c_str(), g_options.m_launchCS, &dllSuffix, &procHookInfo))
	{
		SKSE_ERROR("unknown exe");

		if(usedCustomRuntimeName)
		{
			// hurr durr
			PrintLoaderError("You have customized the runtime name via SKSE64's .ini file. Version errors can usually be fixed by removing the RuntimeName line from the .ini file.");
		}

		return 1;
	}

	if(g_options.m_crcOnly)
		return 0;

	// build dll path
	std::string	dllPath;
	if(dllHasFullPath)
	{
		dllPath = baseDllName;
	}
	else
	{
		dllPath = runtimeDir + "\\" + baseDllName + "_" + dllSuffix + ".dll";
	}

	SKSE_MESSAGE("dll = %s", dllPath.c_str());

	// check to make sure the dll exists
	{
		IFileStream	tempFile;

		if(!tempFile.Open(dllPath.c_str()))
		{
			PrintLoaderError("Couldn't find SKSE64 DLL (%s). Please make sure you have installed SKSE64 correctly and are running it from your Skyrim SE folder.", dllPath.c_str());
			return 1;
		}
	}

	// steam setup
	if(procHookInfo.procType == kProcType_Steam)
	{
		if(g_options.m_launchSteam)
		{
			// if steam isn't running, launch it
			if(!SteamCheckPassive())
			{
				SKSE_MESSAGE("steam not running, launching it");

				if(!SteamLaunch())
				{
					SKSE_WARNING("failed to launch steam");
				}
			}
		}

		// same for standard and nogore
		const char * kAppID = (g_options.m_launchCS == false ? "489830" : "???");

		// set this no matter what to work around launch issues
		SetEnvironmentVariable("SteamGameId", kAppID);

		if(g_options.m_skipLauncher)
		{
			SetEnvironmentVariable("SteamAppID", kAppID);
		}
	}

	// launch the app (suspended)
	STARTUPINFO			startupInfo = { 0 };
	PROCESS_INFORMATION	procInfo = { 0 };

	startupInfo.cb = sizeof(startupInfo);

	if(!CreateProcess(
		procPath.c_str(),
		NULL,	// no args
		NULL,	// default process security
		NULL,	// default thread security
		FALSE,	// don't inherit handles
		CREATE_SUSPENDED,
		NULL,	// no new environment
		NULL,	// no new cwd
		&startupInfo, &procInfo))
	{
		if(GetLastError() == 740)
		{
			PrintLoaderError("Launching %s failed (%d). Please try running skse64_loader as an administrator.", procPath.c_str(), GetLastError());
		}
		else
		{
			PrintLoaderError("Launching %s failed (%d).", procPath.c_str(), GetLastError());
		}

		return 1;
	}

	SKSE_MESSAGE("main thread id = %d", procInfo.dwThreadId);

	// set affinity if requested
	if(g_options.m_affinity)
	{
		SKSE_MESSAGE("setting affinity mask to %016I64X", g_options.m_affinity);

		if(!SetProcessAffinityMask(procInfo.hProcess, g_options.m_affinity))
		{
			SKSE_WARNING("couldn't set affinity mask (%08X)", GetLastError());
		}
	}

	bool	injectionSucceeded = false;
	UInt32	procType = procHookInfo.procType;

	if(g_options.m_forceSteamLoader)
	{
		SKSE_MESSAGE("forcing steam loader");
		procType = kProcType_Steam;
	}

	// inject the dll
	switch(procType)
	{
	case kProcType_Steam:
		{
			std::string	steamHookDllPath = runtimeDir + "\\skse64_steam_loader.dll";

			injectionSucceeded = InjectDLLThread(&procInfo, steamHookDllPath.c_str(), true, g_options.m_noTimeout);
		}
		break;

	case kProcType_Normal:
#if 0
		if(InjectDLL(&procInfo, dllPath.c_str(), &procHookInfo))
		{
			injectionSucceeded = true;
		}
#else
		injectionSucceeded = InjectDLLThread(&procInfo, dllPath.c_str(), true, g_options.m_noTimeout);
#endif
		break;

	default:
		HALT("impossible");
	}

	// start the process if successful
	if(!injectionSucceeded)
	{
		PrintLoaderError("Couldn't inject DLL.");

		SKSE_ERROR("terminating process");

		TerminateProcess(procInfo.hProcess, 0);
	}
	else
	{
		SKSE_MESSAGE("launching");

		if(!ResumeThread(procInfo.hThread))
		{
			SKSE_WARNING("WARNING: something has started the runtime outside of skse64_loader's control.");
			SKSE_WARNING("SKSE64 will probably not function correctly.");
			SKSE_WARNING("Try running skse64_loader as an administrator, or check for conflicts with a virus scanner.");
		}

		if(g_options.m_moduleInfo)
		{
			Sleep(1000 * 3);	// wait 3 seconds

			PrintModuleInfo(procInfo.dwProcessId);
			PrintProcessInfo();
		}

		if(g_options.m_waitForClose)
			WaitForSingleObject(procInfo.hProcess, INFINITE);
	}

	// clean up
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	return 0;
}

static void PrintModuleInfo(UInt32 procID)
{
	HANDLE	snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID);
	if(snap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32	module;

		module.dwSize = sizeof(module);

		if(Module32First(snap, &module))
		{
			do 
			{
				SKSE_MESSAGE("%08Xx%08X %08X %s %s", module.modBaseAddr, module.modBaseSize, module.hModule, module.szModule, module.szExePath);
			}
			while(Module32Next(snap, &module));
		}
		else
		{
			SKSE_ERROR("PrintModuleInfo: Module32First failed (%d)", GetLastError());
		}

		CloseHandle(snap);
	}
	else
	{
		SKSE_ERROR("PrintModuleInfo: CreateToolhelp32Snapshot failed (%d)", GetLastError());
	}
}

static void PrintProcessInfo()
{
	HANDLE	snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(snap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32	proc;

		proc.dwSize = sizeof(PROCESSENTRY32);
		
		if(Process32First(snap, &proc))
		{
			do
			{
				SKSE_MESSAGE("%s", proc.szExeFile);
				proc.dwSize = sizeof(PROCESSENTRY32);
			}
			while (Process32Next(snap, &proc));
		}
		else
		{
			SKSE_ERROR("PrintProcessInfo: Process32First failed (%d)", GetLastError());
		}

		CloseHandle(snap);
	}
	else
	{
		SKSE_ERROR("PrintProcessInfo: CreateToolhelp32Snapshot failed (%d)", GetLastError());
	}
}