#pragma once

#include "RE/ImageSpaceEffect.h"


namespace RE
{
	class BSReloadShaderI
	{
	public:
		inline static const void* RTTI = RTTI_BSReloadShaderI;


		virtual void Unk_00(void) = 0;	// 00
	};
	static_assert(sizeof(BSReloadShaderI) == 0x8);
}
