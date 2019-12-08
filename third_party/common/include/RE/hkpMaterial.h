#pragma once

#include "RE/hkBaseTypes.h"


namespace RE
{
	class hkpMaterial
	{
	public:
		enum class ResponseType : UInt8
		{
			kInvalid,
			kSimpleContact,
			kReporting,
			kNone,

			kTotal
		};


		ResponseType	responseType;				// 00
		UInt8			pad01;						// 01
		hkHalf			rollingFrictionMultiplier;	// 02
		float			friction;					// 04
		float			restitution;				// 08
	};
	static_assert(sizeof(hkpMaterial) == 0xC);
}
