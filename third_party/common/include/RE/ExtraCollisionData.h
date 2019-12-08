#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraCollisionData : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraCollisionData;


		enum { kExtraTypeID = ExtraDataType::kCollisionData };


		struct Data
		{
			UInt32 unk0;	// 0
		};
		static_assert(sizeof(Data) == 0x4);


		virtual ~ExtraCollisionData();													// 00

		// override (BSExtraData)
		virtual ExtraDataType	GetType() const override;								// 01 - { return kCollisionData; }
		virtual bool			IsNotEqual(const BSExtraData* a_rhs) const override;	// 02


		// members
		Data* unk10;	// 10
	};
	static_assert(sizeof(ExtraCollisionData) == 0x18);
}
