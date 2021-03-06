#pragma once

#include "RE/hkRefPtr.h"
#include "RE/NiObject.h"


namespace RE
{
	class hkReferencedObject;


	class bhkRefObject : public NiObject
	{
	public:
		inline static const void* RTTI = RTTI_bhkRefObject;
		inline static const void* Ni_RTTI = NiRTTI_bhkRefObject;


		virtual ~bhkRefObject();							// 00

		// override(NiObject)
		virtual const NiRTTI*	GetRTTI() const override;	// 02

		// add
		virtual void			Unk_25(void);				// 25
		virtual void			Unk_26(void);				// 26


		// members
		hkRefPtr<hkReferencedObject> referencedObject;	// 10
	};
	static_assert(sizeof(bhkRefObject) == 0x18);
}
