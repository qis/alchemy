#pragma once

#include "RE/BaseFormComponent.h"


namespace RE
{
	class TESImageSpaceModifier;


	class TESImageSpaceModifiableForm : public BaseFormComponent
	{
	public:
		inline static const void* RTTI = RTTI_TESImageSpaceModifiableForm;


		virtual ~TESImageSpaceModifiableForm();								// 00

		// override (BaseFormComponent)
		virtual void	Init() override;									// 01 - { imageSpaceModifier = 0; }
		virtual void	ReleaseRefs() override;								// 02 - { return; }
		virtual void	CopyFromBase(BaseFormComponent* a_rhs) override;	// 03


		// members
		TESImageSpaceModifier* imageSpaceModifier;	// 08 - MNAM
	};
	static_assert(sizeof(TESImageSpaceModifiableForm) == 0x10);
}
