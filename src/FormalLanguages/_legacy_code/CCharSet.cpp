//Class Header
#include "../../headers/Formal Languages/CCharSet.h"
//Local
#include "../../headers/Debug.h"
#include "../../headers/Memory.h"
//Namespaces
using namespace ACStdLib;

//Public methods
CCharSet *CCharSet::GetIntersection(const CCharSet &refOther) const
{
	CCharSet *pIntersection;

	auto itLeft = this->ranges.begin();
	auto itRight = refOther.GetRanges().begin();

	pIntersection = new CCharSet;
	while(itLeft != this->ranges.end() && itRight != refOther.GetRanges().end())
	{
		const CRange &refRangeLeft = *itLeft;
		const CRange &refRangeRight = *itRight;
		
		//case 1: right range is subset of left one
		if(refRangeLeft.low <= refRangeRight.low && refRangeLeft.high >= refRangeRight.high)
		{
			pIntersection->Insert(refRangeRight.low, refRangeRight.high);

			++itLeft;
			++itRight;
		}
		//case 2: left range is subset of right one
		else if(refRangeRight.low <= refRangeLeft.low && refRangeRight.high >= refRangeLeft.high)
		{
			pIntersection->Insert(refRangeLeft.low, refRangeLeft.high);

			++itLeft;
			++itRight;
		}
		//case 3: left --- some space ---- right
		else if(refRangeLeft.high < refRangeRight.low)
		{
			++itLeft;
		}
		//case 4: right --- some space --- left
		else if(refRangeRight.high < refRangeLeft.low)
		{
			++itRight;
		}
		else
		{
			ASSERT(0);
		}
	}
	
	return pIntersection;
}
