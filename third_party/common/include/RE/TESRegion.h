#pragma once

#include "RE/BSTList.h"
#include "RE/FormTypes.h"
#include "RE/TESForm.h"


namespace RE
{
	class TESRegionData;


	class TESRegion : public TESForm
	{
	public:
		inline static const void* RTTI = RTTI_TESRegion;


		enum { kTypeID = FormType::Region };


		struct RecordFlags
		{
			enum RecordFlag : UInt32
			{
				kDeleted = 1 << 5,
				kBorderRegion = 1 << 6,
				kIgnored = 1 << 12
			};
		};


		struct RegionArea
		{
			struct Point
			{
				float	x;	// 00
				float	y;	// 04
			};
			static_assert(sizeof(Point) == 0x8);


			struct Unk10
			{
				UInt64	unk00;	// 00
				UInt64	unk08;	// 08
			};
			static_assert(sizeof(Unk10) == 0x10);


			BSSimpleList<Point*>	regionPointListData;	// 00 - RPLD
			Unk10*					unk10;					// 10
			UInt32					unk18;					// 18
			float					xMin;					// 1C
			float					yMin;					// 20
			float					xMax;					// 24
			float					yMax;					// 28
			UInt32					edgeFallOff;			// 2C - RPLI
			UInt32					numPoints;				// 30
			UInt32					unk34;					// 34
		};
		static_assert(sizeof(RegionArea) == 0x38);


		virtual ~TESRegion();								// 00

		// override (TESForm)
		virtual void	ReleaseManagedData(void) override;	// 05
		virtual bool	LoadForm(TESFile* a_mod) override;	// 06
		virtual void	InitItem() override;				// 13

		// add
		virtual bool	IsLoaded() const;					// 3B


		// members
		BSSimpleList<TESRegionData*>*	regionDataEntries;	// 20
		BSSimpleList<RegionArea*>*		regionAreas;		// 28
		TESWorldSpace*					worldspace;			// 30 - WNAM
		UInt64							unk38;				// 38
		UInt64							unk40;				// 40
		UInt32							unk48;				// 48
		UInt32							unk4C;				// 4C
	};
	static_assert(sizeof(TESRegion) == 0x50);
}
