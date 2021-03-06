#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraWornLeft : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraWornLeft;


		enum { kExtraTypeID = ExtraDataType::kWornLeft };


		virtual ~ExtraWornLeft();						// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kWorn; }
	};
	static_assert(sizeof(ExtraWornLeft) == 0x10);
}
