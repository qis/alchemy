#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraEnableStateParent : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraEnableStateParent;


		enum { kExtraTypeID = ExtraDataType::kEnableStateParent };


		virtual ~ExtraEnableStateParent();												// 00

		// override (BSExtraData)
		virtual ExtraDataType	GetType() const override;								// 01 - { return kEnableStateParent; }
		virtual bool			IsNotEqual(const BSExtraData* a_rhs) const override;	// 02 - { return unk14 != a_rhs->unk14 || unk10 != a_rhs->unk10; }


		// members
		UInt8	unk10;	// 10
		UInt8	pad11;	// 11
		UInt16	pad12;	// 12
		UInt32	unk14;	// 14
	};
	static_assert(sizeof(ExtraEnableStateParent) == 0x18);
}
