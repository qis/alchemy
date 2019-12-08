#pragma once


namespace RE
{
	struct TESWaitStopEvent
	{
		UInt8 pad0;	// 0
	};
	static_assert(sizeof(TESWaitStopEvent) == 0x1);
}
