#pragma once

#include "RE/hkbEventBase.h"
#include "RE/hkRefVariant.h"


namespace RE
{
	class hkbEvent : public hkbEventBase
	{
	public:
		// members
		hkRefVariant sender;	// 10
	};
	static_assert(sizeof(hkbEvent) == 0x18);
}
