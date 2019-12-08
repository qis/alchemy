#include "Options.h"

Options g_options;

Options::Options()
	:m_launchCS(false)
	,m_setPriority(false)
	,m_priority(0)
	,m_crcOnly(false)
	,m_optionsOnly(false)
	,m_waitForClose(false)
	,m_verbose(false)
	,m_moduleInfo(false)
	,m_skipLauncher(true)
	,m_launchSteam(false)
	,m_noTimeout(false)
	,m_forceSteamLoader(false)
	,m_affinity(0)
{
	//
}

Options::~Options()
{
	//
}

// disable "switch statement contains 'default' but no 'case' labels"
#pragma warning (push)
#pragma warning (disable : 4065)

bool Options::Read(int argc, char ** argv)
{
	if(argc >= 1)
	{
		// remove app name
		argc--;
		argv++;

		int	freeArgCount = 0;

		while(argc > 0)
		{
			char	* arg = *argv++;
			argc--;

			if(arg[0] == '-')
			{
				// switch
				arg++;

				if(!_stricmp(arg, "editor"))
				{
					m_launchCS = true;
				}
				else if(!_stricmp(arg, "priority"))
				{
					if(argc >= 1)
					{
						arg = *argv++;
						argc--;

						m_setPriority = true;

						if(!_stricmp(arg, "above_normal"))
						{
							m_priority = ABOVE_NORMAL_PRIORITY_CLASS;
						}
						else if(!_stricmp(arg, "below_normal"))
						{
							m_priority = BELOW_NORMAL_PRIORITY_CLASS;
						}
						else if(!_stricmp(arg, "high"))
						{
							m_priority = HIGH_PRIORITY_CLASS;
						}
						else if(!_stricmp(arg, "idle"))
						{
							m_priority = IDLE_PRIORITY_CLASS;
						}
						else if(!_stricmp(arg, "normal"))
						{
							m_priority = NORMAL_PRIORITY_CLASS;
						}
						else if(!_stricmp(arg, "realtime"))
						{
							m_priority = REALTIME_PRIORITY_CLASS;
						}
						else
						{
							m_setPriority = false;

							SKSE_ERROR("couldn't read priority argument (%s)", arg);
							return false;
						}
					}
					else
					{
						SKSE_ERROR("priority not specified");
						return false;
					}
				}
				else if(!_stricmp(arg, "altexe"))
				{
					if(argc >= 1)
					{
						m_altEXE = *argv++;
						argc--;
					}
					else
					{
						SKSE_ERROR("exe path not specified");
						return false;
					}
				}
				else if(!_stricmp(arg, "altdll"))
				{
					if(argc >= 1)
					{
						m_altDLL = *argv++;
						argc--;
					}
					else
					{
						SKSE_ERROR("dll path not specified");
						return false;
					}
				}
				else if(!_stricmp(arg, "crconly"))
				{
					m_crcOnly = true;
				}
				else if(!_stricmp(arg, "h") || !_stricmp(arg, "help"))
				{
					m_optionsOnly = true;
				}
				else if(!_stricmp(arg, "waitforclose"))
				{
					m_waitForClose = true;
				}
				else if(!_stricmp(arg, "v"))
				{
					m_verbose = true;
				}
				else if(!_stricmp(arg, "minfo"))
				{
					m_moduleInfo = true;
				}
				else if(!_stricmp(arg, "noskiplauncher"))
				{
					m_skipLauncher = false;
				}
				else if(!_stricmp(arg, "launchsteam"))
				{
					m_launchSteam = true;
				}
				else if(!_stricmp(arg, "notimeout"))
				{
					m_noTimeout = true;
				}
				else if(!_stricmp(arg, "affinity"))
				{
					if(argc >= 1)
					{
						const char	* maskStr = *argv++;
						argc--;

						if(sscanf_s(maskStr, "%I64i", &m_affinity) != 1)
						{
							SKSE_ERROR("couldn't read affinity mask as an integer (%s)", maskStr);
							return false;
						}
					}
					else
					{
						SKSE_ERROR("affinity mask not specified");
						return false;
					}
				}
				else if(!_stricmp(arg, "forcesteamloader"))
				{
					m_forceSteamLoader = true;
				}
				else
				{
					SKSE_ERROR("unknown switch (%s)", arg);
					return false;
				}
			}
			else
			{
				// free arg

				switch(freeArgCount)
				{
				default:
					SKSE_ERROR("too many free args (%s)", arg);
					return false;
				}
			}
		}
	}

	return Verify();
}

#pragma warning (pop)

void Options::PrintUsage(void)
{
	gLog.SetPrintLevel(IDebugLog::kLevel_VerboseMessage);

	SKSE_MESSAGE("usage: skse64_loader [options]");
	SKSE_MESSAGE("");
	SKSE_MESSAGE("options:");
	SKSE_MESSAGE("  -h, -help - print this options list");
	SKSE_MESSAGE("  -editor - launch the construction set");
	SKSE_MESSAGE("  -priority <level> - set the launched program\'s priority");
	SKSE_MESSAGE("    above_normal");
	SKSE_MESSAGE("    below_normal");
	SKSE_MESSAGE("    high");
	SKSE_MESSAGE("    idle");
	SKSE_MESSAGE("    normal");
	SKSE_MESSAGE("    realtime");
	SKSE_MESSAGE("  -altexe <path> - set alternate exe path");
	SKSE_MESSAGE("  -altdll <path> - set alternate dll path");
	SKSE_MESSAGE("  -crconly - just identify the EXE, don't launch anything");
	SKSE_MESSAGE("  -waitforclose - wait for the launched program to close");
	SKSE_MESSAGE("  -v - print verbose messages to the console");
	SKSE_MESSAGE("  -minfo - log information about the DLLs loaded in to the target process");
	SKSE_MESSAGE("  -noskiplauncher - does not skip the default Bethesda launcher window");
	SKSE_MESSAGE("                    note: specifying this option may cause compatibility problems");
	SKSE_MESSAGE("  -launchsteam - attempt to launch steam if it is not running");
	SKSE_MESSAGE("  -affinity <mask> - set the processor affinity mask");
	SKSE_MESSAGE("  -forcesteamloader - override exe type detection and use steam loader");
}

bool Options::Verify(void)
{
	// nothing to verify currently

	return true;
}
