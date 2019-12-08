#pragma once


namespace RE
{
	class TESObjectREFR;


	struct TESInitScriptEvent
	{
		TESObjectREFR* objRef;	// 0
	};
	static_assert(sizeof(TESInitScriptEvent) == 0x8);
}
