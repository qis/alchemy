#pragma once

#include "RE/BaseFormComponent.h"


namespace RE
{
	class TESObjectARMO;


	class BGSSkinForm : public BaseFormComponent
	{
	public:
		inline static const void* RTTI = RTTI_BGSSkinForm;


		virtual ~BGSSkinForm();												// 00

		// override (BaseFormComponent)
		virtual void	Init() override;									// 01
		virtual void	ReleaseRefs() override;								// 02 - { return; }
		virtual void	CopyFromBase(BaseFormComponent* a_rhs) override;	// 03


		// members
		TESObjectARMO* skin;	// 08 - WNAM
	};
	static_assert(sizeof(BGSSkinForm) == 0x10);
}
