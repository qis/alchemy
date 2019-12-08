#pragma once


namespace RE
{
	namespace BSScript
	{
		struct StatsEvent
		{
		public:
			UInt32	numStacks;			// 00
			UInt32	numSuspendedStacks;	// 04
			UInt32	unk08;				// 08
			UInt32	unk0C;				// 0C
		};
		static_assert(sizeof(StatsEvent) == 0x10);
	}
}
