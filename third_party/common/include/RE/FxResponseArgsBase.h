#pragma once


namespace RE
{
	class GFxValue;


	class FxResponseArgsBase
	{
	public:
		virtual ~FxResponseArgsBase() = default;			// 00

		// add
		virtual UInt32 GetValues(GFxValue** a_params) = 0;	// 01
	};
	static_assert(sizeof(FxResponseArgsBase) == 0x8);
}
