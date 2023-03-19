//Class Header
#include "../../../headers/Math/Geometry/CRect.h"
//Local
#include "../../../headers/Debug.h"
//Namespaces
using namespace ACStdLib;

//Public methods
CRect CRect::Intersect(const CRect &refRect) const
{
	CRect result;

	result.x() = MAX(this->x(), refRect.x());
	result.y() = MAX(this->y(), refRect.y());
	result.width() = MIN(this->GetRight(), refRect.GetRight()) - result.x();
	result.height() = MIN(this->GetBottom(), refRect.GetBottom()) - result.y();
	
	return result;
}