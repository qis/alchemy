#pragma once


namespace RE
{
	class NiPoint2
	{
	public:
		float	x;	// 0
		float	y;	// 4
	};
	static_assert(sizeof(NiPoint2) == 0x8);
}
