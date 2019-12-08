#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraOcclusionPlaneRefData : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraOcclusionPlaneRefData;


		enum { kExtraTypeID = ExtraDataType::kOcclusionPlaneRefData };


		struct Data
		{
			UInt64	unk00;	// 00
			UInt64	unk08;	// 08
		};
		static_assert(sizeof(Data) == 0x10);


		virtual ~ExtraOcclusionPlaneRefData();											// 00

		// override (BSExtraData)
		virtual ExtraDataType	GetType() const override;								// 01 - { return kOcclusionPlaneRefData; }
		virtual bool			IsNotEqual(const BSExtraData* a_rhs) const override;	// 02


		// members
		Data* unk10;	// 10
	};
	static_assert(sizeof(ExtraOcclusionPlaneRefData) == 0x18);
}
