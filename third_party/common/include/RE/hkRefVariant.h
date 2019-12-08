#pragma once

#include "RE/hkReferencedObject.h"
#include "RE/hkRefPtr.h"


namespace RE
{
	class hkRefVariant : public hkRefPtr<hkReferencedObject>
	{
	public:
	};
	static_assert(sizeof(hkRefVariant) == 0x8);
}
