#pragma once

#include "RE/BGSPreloadable.h"
#include "RE/BSTArray.h"
#include "RE/BSTList.h"
#include "RE/FileHash.h"
#include "RE/FormTypes.h"
#include "RE/TESForm.h"


namespace RE
{
	class BGSDebris :
		public TESForm,			// 00
		public BGSPreloadable	// 20
	{
	public:
		inline static const void* RTTI = RTTI_BGSDebris;


		enum { kTypeID = FormType::Debris };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kDeleted = 1 << 5,
				kIgnored = 1 << 12
			};
		};


		struct Model
		{
			struct Data	// DATA
			{
				enum class Flag : UInt8
				{
					kNone = 0,
					kHasCollisionData = 1 << 0
				};


				UInt8	percentage;	// 0
				Flag	flags;		// 1
				UInt16	pad2;		// 2
				UInt32	pad4;		// 4
			};
			static_assert(sizeof(Data) == 0x8);


			Data				data;				// 00 - DATA
			const char*			modelFileName;		// 10
			BSTArray<FileHash*>	textureFileHashes;	// 18 - MODT
		};
		static_assert(sizeof(Model) == 0x28);


		virtual ~BGSDebris();								// 00

		// override (TESForm)
		virtual void	InitDefaults() override;			// 04 - { return; }
		virtual void	ReleaseManagedData() override;		// 05
		virtual bool	LoadForm(TESFile* a_mod) override;	// 06


		// members
		BSSimpleList<Model*> models;	// 28
	};
	static_assert(sizeof(BGSDebris) == 0x38);
}
