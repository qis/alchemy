#pragma once

#include "RE/GFxValue.h"


namespace RE
{
	struct BottomBar
	{
		GFxValue obj;	// 00 - kObject
	};
	static_assert(sizeof(BottomBar) == 0x18);
}
