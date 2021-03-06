#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraShouldWear : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraShouldWear;


		enum { kExtraTypeID = ExtraDataType::kShouldWear };


		virtual ~ExtraShouldWear();						// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kShouldWear; }


		// members
		UInt64 unk10;	// 10
	};
	static_assert(sizeof(ExtraShouldWear) == 0x18);
}
