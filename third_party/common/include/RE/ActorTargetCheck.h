#pragma once

#include "RE/ProcessLists/GetActorsFilter.h"


namespace RE
{
	class ActorTargetCheck : public ProcessLists::GetActorsFilter
	{
	public:
		inline static const void* RTTI = RTTI_ActorTargetCheck;


		virtual ~ActorTargetCheck();		// 00

		// override (GetActorsFilter)
		virtual void Unk_01(void) override;	// 01


		// members
		UInt64	unk08;	// 08
		UInt64	unk10;	// 10
		UInt64	unk18;	// 18
	};
	static_assert(sizeof(ActorTargetCheck) == 0x20);
}
