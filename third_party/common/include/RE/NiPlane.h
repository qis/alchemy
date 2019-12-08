#pragma once

#include "RE/NiPoint3.h"


namespace RE
{
	class NiPlane
	{
	public:
		NiPoint3	normal;		// 00
		float		constant;	// 0C
	};
	static_assert(sizeof(NiPlane) == 0x10);
}
