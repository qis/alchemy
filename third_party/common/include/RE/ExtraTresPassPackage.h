#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class TESForm;


	class ExtraTresPassPackage : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraTresPassPackage;


		enum { kExtraTypeID = ExtraDataType::kTresPassPackage };


		virtual ~ExtraTresPassPackage();				// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kTresPassPackage; }


		// members
		TESForm*	unk10;	// 10
		UInt64		unk18;	// 18
	};
	static_assert(sizeof(ExtraTresPassPackage) == 0x20);
}
