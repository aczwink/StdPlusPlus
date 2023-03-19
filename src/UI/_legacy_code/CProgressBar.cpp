//Class Header
#include "../../../headers/TUI/Widgets/CProgressBar.h"
//Local
#include "../../../headers/TUI/CDrawContext.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::TUI;

//Eventhandlers
void CProgressBar::OnDraw()
{
	uint8 percentage;
	int16 x, y;
	uint16 progressLength, nChars;
	
	CDrawContext dc(*this);

	const CRect &refRect = this->GetRect();

	x = refRect.x();
	y = refRect.y();
	progressLength = refRect.width() - 2;
	percentage = (uint8)(this->progress * 100);
	
	//draw the ends
	dc.OutputCodePoint(x, y, '[');
	dc.OutputCodePoint(refRect.GetRight() - 1, y, ']');

	//draw the progress
	nChars = uint16(this->progress * progressLength);

	for(uint16 i = 0; i < nChars; i++)
	{
		dc.OutputCodePoint(x + i + 1, y, '>');
	}

	//draw the number
	if(percentage < 10)
	{
		x += progressLength / 2;
		dc.OutputCodePoint(x++, y, percentage + '0');
	}
	else if(percentage < 100)
	{
		x += progressLength / 2 - 1;
		dc.OutputCodePoint(x++, y, ((percentage / 10) % 10) + '0');
		dc.OutputCodePoint(x++, y, (percentage % 10) + '0');
	}
	else
	{
		x += progressLength / 2 - 2;
		dc.OutputCodePoint(x++, y, '1');
		dc.OutputCodePoint(x++, y, '0');
		dc.OutputCodePoint(x++, y, '0');
	}

	dc.OutputCodePoint(x, y, '%');
}