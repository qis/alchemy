#pragma once

#include "RE/BGSBipedObjectForm.h"
#include "RE/BGSBlockBashData.h"
#include "RE/BGSDestructibleObjectForm.h"
#include "RE/BGSEquipType.h"
#include "RE/BGSKeywordForm.h"
#include "RE/BGSPickupPutdownSounds.h"
#include "RE/BSTArray.h"
#include "RE/FormTypes.h"
#include "RE/TESBipedModelForm.h"
#include "RE/TESBoundObject.h"
#include "RE/TESDescription.h"
#include "RE/TESEnchantableForm.h"
#include "RE/TESFullName.h"
#include "RE/TESRaceForm.h"
#include "RE/TESValueForm.h"
#include "RE/TESWeightForm.h"


namespace RE
{
	class TESObjectARMO :
		public TESBoundObject,				// 000
		public TESFullName,					// 030
		public TESRaceForm,					// 040
		public TESEnchantableForm,			// 050
		public TESValueForm,				// 068
		public TESWeightForm,				// 078
		public BGSDestructibleObjectForm,	// 088
		public BGSPickupPutdownSounds,		// 098
		public TESBipedModelForm,			// 0B0
		public BGSEquipType,				// 1A0
		public BGSBipedObjectForm,			// 1B0
		public BGSBlockBashData,			// 1C0
		public BGSKeywordForm,				// 1D8
		public TESDescription				// 1F0
	{
	public:
		inline static const void* RTTI = RTTI_TESObjectARMO;


		enum { kTypeID = FormType::Armor };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kNonPlayable = 1 << 2,
				kDeleted = 1 << 5,
				kShield = 1 << 6,
				kIgnored = 1 << 12
			};
		};


		// override (TESBoundObject)
		virtual void		InitDefaults() override;						// 04
		virtual bool		LoadForm(TESFile* a_mod) override;				// 06
		virtual void		SaveBuffer(BGSSaveFormBuffer* a_buf) override;	// 0E
		virtual void		LoadBuffer(BGSLoadFormBuffer* a_buf) override;	// 0F
		virtual void		InitItem() override;							// 13
		virtual TESFile*	GetFinalSourceFile() override;					// 14 - { return templateArmor ? templateArmor->GetFile(-1) : GetFile(-1); }
		virtual void		CopyFrom(TESForm* a_srcForm) override;			// 2F

		float GetArmorRating();


		// members
		UInt32						armorRating;	// 200 - DNAM - CK value * 100 as a UInt32
		UInt32						pad204;			// 204
		BSTArray<TESObjectARMA*>	armature;		// 208
		TESObjectARMO*				templateArmor;	// 220 - TNAM
	};
	static_assert(sizeof(TESObjectARMO) == 0x228);
}
