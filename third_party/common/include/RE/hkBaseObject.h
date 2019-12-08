#pragma once


namespace RE
{
	class hkBaseObject
	{
	public:
		inline static const void* RTTI = RTTI_hkBaseObject;


		virtual ~hkBaseObject();	// 00
	};
	static_assert(sizeof(hkBaseObject) == 0x8);
}
