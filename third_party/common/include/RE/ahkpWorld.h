#pragma once

#include "RE/hkpWorld.h"


namespace RE
{
	class ahkpWorld : public hkpWorld
	{
	public:
		inline static const void* RTTI = RTTI_ahkpWorld;


		virtual ~ahkpWorld();	// 00


		// members
		UInt64	unk430;	// 430
		UInt64	unk438;	// 438
	};
	static_assert(sizeof(ahkpWorld) == 0x440);
}