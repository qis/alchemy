#pragma once

#include "RE/BSTEvent.h"


namespace RE
{
	class TESObjectREFR;
	class InventoryEntryData;


	struct Inventory
	{
	public:
		struct Event
		{
		public:
			TESObjectREFR*		objRefr;	// 00
			InventoryEntryData* entryData;	// 08
			SInt32				newCount;	// 10
			SInt32				prevCount;	// 14
		};
		static_assert(sizeof(Event) == 0x18);


		static BSTEventSource<Inventory::Event>* GetEventSource();
	};
}
