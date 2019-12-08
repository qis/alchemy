#pragma once


namespace RE
{
	struct PoisonedWeapon
	{
	public:
		struct Event
		{
		public:
			UInt32 pad0;	// 0
		};
		static_assert(sizeof(Event) == 0x4);
	};
}
