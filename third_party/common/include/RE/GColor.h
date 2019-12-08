#pragma once


namespace RE
{
	class GColor
	{
	public:
		struct RGB32
		{
			UInt8	blue;	// 0
			UInt8	green;	// 1
			UInt8	red;	// 2
			UInt8	alpha;	// 3
		};
		static_assert(sizeof(RGB32) == 0x4);


		union ColorData
		{
			RGB32	channels;
			UInt32	raw;
		};
		static_assert(sizeof(ColorData) == 0x4);


		// members
		ColorData colorData;	// 0
	};
	static_assert(sizeof(GColor) == 0x4);
}
