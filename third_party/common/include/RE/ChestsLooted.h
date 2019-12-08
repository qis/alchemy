#pragma once

#include "RE/BSTEvent.h"


namespace RE
{
	struct ChestsLooted
	{
	public:
		struct Event
		{
		public:
			UInt64 pad0;	// 0
		};
		static_assert(sizeof(Event) == 0x8);


		static BSTEventSource<Event>*	GetEventSource();
		static void						SendEvent();
	};
}
