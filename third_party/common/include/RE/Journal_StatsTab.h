#pragma once

#include "RE/JournalTab.h"


namespace RE
{
	class Journal_StatsTab : public JournalTab
	{
	public:
		inline static const void* RTTI = RTTI_Journal_StatsTab;


		virtual ~Journal_StatsTab();								// 00

		// override (JournalTab)
		virtual void Accept(CallbackProcessor* a_cbReg) override;	// 01
	};
	static_assert(sizeof(Journal_StatsTab) == 0x18);
}
