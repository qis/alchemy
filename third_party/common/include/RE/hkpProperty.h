#pragma once


namespace RE
{
	struct hkpPropertyValue
	{
		UInt64 data;	// 0
	};
	static_assert(sizeof(hkpPropertyValue) == 0x8);


	class hkpProperty
	{
	public:
		UInt32				key;				// 00
		UInt32				alignmentPadding;	// 04
		hkpPropertyValue	value;				// 08
	};
	static_assert(sizeof(hkpProperty) == 0x10);
}
