#pragma once


namespace RE
{
	class TESObjectCELL;


	struct TESCellFullyLoadedEvent
	{
		TESObjectCELL* cell;
	};
	static_assert(sizeof(TESCellFullyLoadedEvent) == 0x8);
}
