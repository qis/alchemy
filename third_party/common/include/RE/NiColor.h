#pragma once


namespace RE
{
	class NiColor
	{
	public:
		NiColor();
		NiColor(float a_red, float a_green, float a_blue);


		// members
		float	red;	// 0
		float	green;	// 4
		float	blue;	// 8
	};
	static_assert(sizeof(NiColor) == 0xC);


	class NiColorA
	{
	public:
		NiColorA();
		NiColorA(float a_red, float a_green, float a_blue, float a_alpha);


		// members
		float	red;	// 00
		float	green;	// 04
		float	blue;	// 08
		float	alpha;	// 0C
	};
	static_assert(sizeof(NiColorA) == 0x10);
}
