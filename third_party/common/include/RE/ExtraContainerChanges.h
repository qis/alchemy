#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class InventoryChanges;


	class ExtraContainerChanges : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraContainerChanges;


		enum { kExtraTypeID = ExtraDataType::kContainerChanges };


		virtual	~ExtraContainerChanges();				// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kContainerChanges; }


		// members
		InventoryChanges* changes;	// 10
	};
	static_assert(sizeof(ExtraContainerChanges) == 0x18);
}
