#pragma once

#include "RE/hkArray.h"
#include "RE/hkpCdPointCollector.h"
#include "RE/hkpRootCdPoint.h"


namespace RE
{
	class hkpAllCdPointCollector : public hkpCdPointCollector
	{
	public:
		inline static const void* RTTI = RTTI_hkpAllCdPointCollector;


		virtual ~hkpAllCdPointCollector();								// 00

		// override (hkpCdPointCollector)
		virtual void	AddCdPoint(const hkpCdPoint& a_point) override;	// 01
		virtual void	Reset() override;								// 02 - { hits.size = 0; earlyOutDistance = 0x7F7FFFEE; }


		// members
		hkInplaceArray<hkpRootCdPoint, 8> hits;	// 010
	};
	static_assert(sizeof(hkpAllCdPointCollector) == 0x220);
}
