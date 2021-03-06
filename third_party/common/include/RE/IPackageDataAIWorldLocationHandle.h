#pragma once

#include "RE/IAIWorldLocationHandle.h"
#include "RE/IPackageData.h"


namespace RE
{
	class IPackageDataAIWorldLocationHandle :
		public IAIWorldLocationHandle,	// 00
		public IPackageData				// 08
	{
	public:
		inline static const void* RTTI = RTTI_IPackageDataAIWorldLocationHandle;


		virtual ~IPackageDataAIWorldLocationHandle();	// 00
	};
	static_assert(sizeof(IPackageDataAIWorldLocationHandle) == 0x10);
}
