#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraProcessMiddleLow : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraProcessMiddleLow;


		enum { kExtraTypeID = ExtraDataType::kProcessMiddleLow };


		virtual ~ExtraProcessMiddleLow();				// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kProcessMiddleLow; }


		// members
		UInt64 unk10;	// 10
	};
	static_assert(sizeof(ExtraProcessMiddleLow) == 0x18);
}
