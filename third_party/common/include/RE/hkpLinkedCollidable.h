#pragma once

#include "RE/hkArray.h"
#include "RE/hkpCollidable.h"


namespace RE
{
	struct hkpAgentNnEntry;


	class hkpLinkedCollidable : public hkpCollidable
	{
	public:
		struct CollisionEntry
		{
			hkpAgentNnEntry*		agentEntry;	// 00
			hkpLinkedCollidable*	partner;	// 08
		};
		static_assert(sizeof(CollisionEntry) == 0x10);


		hkArray<CollisionEntry> collisionEntries;	// 70
	};
	static_assert(sizeof(hkpLinkedCollidable) == 0x80);
}
