#pragma once

#include "RE/NiSmartPointer.h"


namespace RE
{
	class TESObjectREFR;


	struct TESLockChangedEvent
	{
		NiPointer<TESObjectREFR> ref;	// 0
	};
	static_assert(sizeof(TESLockChangedEvent) == 0x8);
}
