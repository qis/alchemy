#pragma once


namespace RE
{
	class hkpBroadPhaseHandle
	{
	public:
		UInt32 id;	// 0
	};
	static_assert(sizeof(hkpBroadPhaseHandle) == 0x4);
}
