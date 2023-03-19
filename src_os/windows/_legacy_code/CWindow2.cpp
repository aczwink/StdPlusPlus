//Class Header
#include "CWindow.h"
//SJC
#include <SJCWinLib.h>
//Namespaces
using namespace SJCLib;
using namespace SJCTUILib;

//Public Functions
void CWindow::Clear()
{
	SJCWinLib::CConsole console;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	console.GetStdOut();
	console.GetScreenBufferInfo(&csbi);
	console.FillOutputCharacter(' ', this->origin.x, this->origin.y, this->GetLength(this->origin, this->end));
}

char CWindow::GetChar()
{
	SJCWinLib::CConsole console;
	uint32 oldMode;
	INPUT_RECORD record;

	console.GetStdIn();
	oldMode = console.GetMode();
	console.SetMode(0);

	while(true)
	{
		console.ReadInput(&record, 1);
		
		if((record.EventType == KEY_EVENT) && record.Event.KeyEvent.bKeyDown)
		{
			console.SetMode(oldMode);
			return record.Event.KeyEvent.uChar.AsciiChar;
		}
	}

	console.SetMode(oldMode);
	return -1;
}

bool CWindow::Confirm(CString msg)
{
	char c = 0;
	
	stdOut << msg << " [y/n] ";

	while(!(c == 'y' || c == 'n'))
	{
		c = this->GetChar();
	}

	return c == 'y';
}

uint32 CWindow::GetLength(const CCoord &refStart, const CCoord &refEnd) const
{
	if(refStart.y > refEnd.y)
		return -1;
	if(refStart.y == refEnd.y)
		return refEnd.y - refStart.y;
	
	return refStart.x + (refEnd.y - refStart.y - 1) * this->end.x + refEnd.x;
}

void CWindow::Print(uint16 x, uint16 y, CString text)
{
	SJCWinLib::CConsole console;
	COORD coord;

	coord.X = x;
	coord.Y = y;

	console.GetStdOut();
	console.SetCursorPosition(coord);
	console.Write(text);
}