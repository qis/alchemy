#pragma once

#include "RE/BSFixedString.h"
#include "RE/BSString.h"
#include "RE/Condition.h"
#include "RE/FormTypes.h"
#include "RE/TESForm.h"


namespace RE
{
	class TESIdleForm : public TESForm
	{
	public:
		inline static const void* RTTI = RTTI_TESIdleForm;


		enum { kTypeID = FormType::Idle };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kDeleted = 1 << 5,
				kIgnored = 1 << 12
			};
		};


		struct Data	// DATA
		{
			enum class Flag : UInt8
			{
				kNone = 0,
				kParent = 1 << 0,
				kSequence = 1 << 1,
				kNoAttacking = 1 << 2,
				kBlocking = 1 << 3
			};


			struct LoopingSeconds
			{
				UInt8	min;	// 0
				UInt8	max;	// 1
			};
			static_assert(sizeof(LoopingSeconds) == 0x2);


			LoopingSeconds	loopingSeconds;				// 0
			Flag			flags;						// 2
			UInt8			animationGroupSelection;	// 3
			UInt32			replayDelay;				// 4
		};
		static_assert(sizeof(Data) == 0x8);


		struct RelatedIdleAnimations	// ANAM
		{
			TESIdleForm*	parent;				// 00
			TESIdleForm*	previousSibling;	// 08
		};
		static_assert(sizeof(RelatedIdleAnimations) == 0x10);


		virtual ~TESIdleForm();														// 00

		// override (TESForm)
		virtual void		InitDefaults() override;								// 04
		virtual void		ReleaseManagedData() override;							// 05
		virtual bool		LoadForm(TESFile* a_mod) override;						// 06
		virtual TESForm*	DupulicateForm(void* a_arg1, void* a_arg2) override;	// 09
		virtual void		InitItem() override;									// 13
		virtual const char*	GetEditorID() override;									// 32 - { return editorID.empty() ? "" : editorID.c_str(); }
		virtual bool		SetEditorID(const char* a_str) override;				// 33


		// members
		Condition				conditions;				// 20
		Data					data;					// 28 - DATA
		UInt64					unk30;					// 30
		RelatedIdleAnimations	relatedIdleAnimations;	// 38 - ANAM
		BSFixedString			fileName;				// 48 - DNAM
		BSFixedString			animationEvent;			// 50 - ENAM
		BSString				editorID;				// 58 - EDID
	};
	static_assert(sizeof(TESIdleForm) == 0x68);
}
