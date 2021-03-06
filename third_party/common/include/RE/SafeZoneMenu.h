#pragma once

#include "RE/IMenu.h"


namespace RE
{
	class SafeZoneMenu : public IMenu
	{
	public:
		inline static const void* RTTI = RTTI_SafeZoneMenu;


		virtual ~SafeZoneMenu();	// 00
	};
	static_assert(sizeof(SafeZoneMenu) == 0x30);
}
