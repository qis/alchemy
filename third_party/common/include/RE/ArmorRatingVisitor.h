#pragma once

#include "RE/ArmorRatingVisitorBase.h"
#include "RE/BSTArray.h"


namespace RE
{
	class ArmorRatingVisitor : public ArmorRatingVisitorBase
	{
	public:
		virtual ~ArmorRatingVisitor();											// 00

		// override (ArmorRatingVisitorBase)
		virtual bool	HaveNotVisitedArmor(TESObjectARMO* a_armor) override;	// 04
		virtual void	VisitArmor(TESObjectARMO* a_armor) override;			// 05


		// members
		BSScrapArray<TESObjectARMO*> armors;	// 20
	};
	static_assert(sizeof(ArmorRatingVisitor) == 0x40);
}
