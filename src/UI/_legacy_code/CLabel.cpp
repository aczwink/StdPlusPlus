//Class Header
#include "../../../headers/TUI/Widgets/CLabel.h"
//Local
#include "../../../headers/TUI/CDrawContext.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::TUI;

//Eventhandlers
void CLabel::OnDraw()
{
	int16 x, y;
	
	CDrawContext dc(*this);
	
	const CRect &refRect = this->GetRect();

	x = refRect.x();
	y = refRect.y();
	
	for(uint32 codePoint : this->text)
	{
		dc.OutputCodePoint(x++, y, codePoint);

		if(x >= refRect.GetRight())
		{
			x = refRect.x();
			y++;
		}
	}
}