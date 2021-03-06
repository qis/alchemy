#pragma once

#include "RE/BGSKeyword.h"
#include "RE/FormTypes.h"


namespace RE
{
	class BGSLocationRefType : public BGSKeyword
	{
	public:
		inline static const void* RTTI = RTTI_BGSLocationRefType;


		enum { kTypeID = FormType::LocationRefType };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kDeleted = 1 << 5,
				kIgnored = 1 << 12
			};
		};


		virtual ~BGSLocationRefType();		// 00

		// override (BGSKeyword)
		virtual void InitItem() override;	// 13
	};
	static_assert(sizeof(BGSLocationRefType) == 0x28);
}
