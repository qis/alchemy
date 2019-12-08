#pragma once

#include "RE/BSFixedString.h"


namespace RE
{
	struct TESTrackedStatsEvent
	{
		BSFixedString	stat;	// 00
		UInt32			value;	// 08
		UInt32			pad0C;	// 0C
	};
	static_assert(sizeof(TESTrackedStatsEvent) == 0x10);
}
