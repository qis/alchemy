#pragma once

#include <cstdarg>

#include "RE/BSString.h"
#include "RE/BSTSingleton.h"


namespace RE
{
	class ConsoleManager : public BSTSingletonSDM<ConsoleManager>
	{
	public:
		static ConsoleManager*	GetSingleton();
		static bool				IsConsoleMode();

		void Print(const char* a_fmt, ...);
		void VPrint(const char* a_fmt, std::va_list a_args);


		// members
		char		lastMessage[0x400];	// 001
		UInt8		pad401;				// 401
		UInt16		pad402;				// 402
		UInt32		pad404;				// 404
		BSString	buffer;				// 408
	};
	static_assert(offsetof(ConsoleManager, lastMessage) == 0x1);
	static_assert(sizeof(ConsoleManager) == 0x418);
}
