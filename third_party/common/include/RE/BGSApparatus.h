#pragma once

#include "RE/FormTypes.h"
#include "RE/TESDescription.h"
#include "RE/TESObjectMISC.h"
#include "RE/TESQualityForm.h"


namespace RE
{
	class BGSApparatus :
		public TESObjectMISC,	// 000
		public TESQualityForm,	// 100
		public TESDescription	// 110
	{
	public:
		inline static const void* RTTI = RTTI_BGSApparatus;


		enum { kTypeID = FormType::Apparatus };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kDeleted = 1 << 5,
				kIgnored = 1 << 12
			};
		};


		virtual ~BGSApparatus();						// 00

		// override (TESObjectMISC)
		virtual bool LoadForm(TESFile* a_mod) override;	// 06
	};
	static_assert(sizeof(BGSApparatus) == 0x120);
}
