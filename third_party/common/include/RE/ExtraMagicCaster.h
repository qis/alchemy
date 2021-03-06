#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"
#include "RE/MagicCaster.h"


namespace RE
{
	class ExtraMagicCaster :
		public BSExtraData,	// 00
		public MagicCaster	// 10
	{
	public:
		inline static const void* RTTI = RTTI_ExtraMagicCaster;


		enum { kExtraTypeID = ExtraDataType::kMagicCaster };


		virtual ~ExtraMagicCaster();					// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kMagicCaster; }
	};
	static_assert(sizeof(ExtraMagicCaster) == 0x58);
}
