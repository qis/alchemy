#pragma once

#include "RE/ExtraDataTypes.h"
#include "RE/ExtraLight.h"
#include "RE/NiSmartPointer.h"


namespace RE
{
	class NiRefObject;


	class ExtraMagicLight : public ExtraLight
	{
	public:
		inline static const void* RTTI = RTTI_ExtraMagicLight;


		enum { kExtraTypeID = ExtraDataType::kMagicLight };


		virtual ~ExtraMagicLight();						// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kMagicLight; }
	};
	static_assert(sizeof(ExtraMagicLight) == 0x18);
}
