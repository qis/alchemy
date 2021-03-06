#pragma once

#include "RE/BSExtraData.h"
#include "RE/ExtraDataTypes.h"


namespace RE
{
	class ExtraAnimNoteReceiver : public BSExtraData
	{
	public:
		inline static const void* RTTI = RTTI_ExtraAnimNoteReceiver;


		enum { kExtraTypeID = ExtraDataType::kAnimNoteReceiver };


		virtual ~ExtraAnimNoteReceiver();				// 00

		// override (BSExtraData)
		virtual ExtraDataType GetType() const override;	// 01 - { return kAnimNoteReceiver; }


		// members
		void* unk10;	// 10
	};
	static_assert(sizeof(ExtraAnimNoteReceiver) == 0x18);
}
