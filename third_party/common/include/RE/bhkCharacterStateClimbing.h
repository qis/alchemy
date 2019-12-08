#pragma once

#include "RE/bhkCharacterState.h"


namespace RE
{
	class bhkCharacterStateClimbing : public bhkCharacterState
	{
	public:
		inline static const void* RTTI = RTTI_bhkCharacterStateClimbing;


		virtual ~bhkCharacterStateClimbing();						// 00

		// override (bhkCharacterState)
		virtual hkpCharacterStateType	GetType() const override;	// 03 - { return kClimbing; }
		virtual void					Unk_08(void) override;		// 08
	};
	static_assert(sizeof(bhkCharacterStateClimbing) == 0x10);
}