//Class Header
#include "../../../../headers/TUI/Render Targets/CConsoleRenderTarget.h"
//Global
#include <Windows.h>
//Local
#include "../../../../headers/Debug.h"
#include "../../../../headers/Memory.h"
#include "../../../../headers/Containers/Strings/String.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::TUI;

//Local functions
static WORD MapBackgroundColor(const CColor &refBackground)
{
	WORD attrib;

	attrib = 0;

	if(refBackground.r >= 0.5f)
		attrib |= BACKGROUND_RED;
	if(refBackground.g >= 0.5f)
		attrib |= BACKGROUND_GREEN;
	if(refBackground.b >= 0.5f)
		attrib |= BACKGROUND_BLUE;

	if(refBackground.r >= 0.8f || refBackground.g >= 0.8f || refBackground.b >= 0.8f)
		attrib |= BACKGROUND_INTENSITY;

	return attrib;
}

static WORD MapForegroundColor(const CColor &refBackground)
{
	WORD attrib;

	attrib = 0;

	if(refBackground.r >= 0.5f)
		attrib |= FOREGROUND_RED;
	if(refBackground.g >= 0.5f)
		attrib |= FOREGROUND_GREEN;
	if(refBackground.b >= 0.5f)
		attrib |= FOREGROUND_BLUE;

	if(refBackground.r >= 0.8f || refBackground.g >= 0.8f || refBackground.b >= 0.8f)
		attrib |= FOREGROUND_INTENSITY;

	return attrib;
}

static WORD GetTextAttributes(const CColor &refForeground, const CColor &refBackground)
{
	return MapBackgroundColor(refBackground) | MapForegroundColor(refForeground);
}

//Constructor
CConsoleRenderTarget::CConsoleRenderTarget()
{
	//enable window events
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT);

	//get front buffer
	this->pFrontBuffer = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	this->pBackBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	//disable auto scroll
	SetConsoleMode(this->pFrontBuffer, 0);
	SetConsoleMode(this->pBackBuffer, 0);
	
	this->ResizeView();
}

//Public methods
void CConsoleRenderTarget::Clear()
{
	uint16 nCols, nRows;
	DWORD nCharsWritten;
	COORD coord = {0};
	
	nCols = this->GetNumberOfColumns();
	nRows = this->GetNumberOfRows();
	
	//clear back buffer
	for(uint16 row = 0; row < nRows; row++)
	{
		coord.Y = row;
		SetConsoleCursorPosition(this->pBackBuffer, coord);
		
		for(uint16 col = 0; col < nCols; col++)
		{
			WriteConsoleA(this->pBackBuffer, " ", 1, &nCharsWritten, nullptr);
		}
	}
	
	//FillConsoleOutputCharacterA(this->pBackBuffer, ' ', nCells, coord, &nCharsWritten);
}

uint16 CConsoleRenderTarget::GetNumberOfColumns() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(this->pFrontBuffer, &csbi);
	
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

uint16 CConsoleRenderTarget::GetNumberOfRows() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(this->pFrontBuffer, &csbi);

	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void CConsoleRenderTarget::OutputCodePoint(uint16 x, uint16 y, uint32 codePoint)
{
	DWORD numberOfCharsWritten;
	COORD coord;
	
	coord.X = x;
	coord.Y = y;
	
	SetConsoleCursorPosition(this->pBackBuffer, coord);
	WriteConsoleW(this->pBackBuffer, &codePoint, 1, &numberOfCharsWritten, nullptr);
}

bool CConsoleRenderTarget::PeekEvent(CTuple<EEventType, void *> &refEvent)
{
	HANDLE hStdIn;
	DWORD nEvents, nEventsRead;
	INPUT_RECORD ir;
	
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	GetNumberOfConsoleInputEvents(hStdIn, &nEvents);
	
	while(nEvents--)
	{
		ReadConsoleInputW(hStdIn, &ir, 1, &nEventsRead);
		if(nEventsRead != 1)
		{
			GetNumberOfConsoleInputEvents(hStdIn, &nEvents);
			continue;
		}

		switch(ir.EventType)
		{
		case KEY_EVENT:
			{
				EEventType eventType;
				/*SKeyEvent *pEventData;
				
				eventType = ir.Event.KeyEvent.bKeyDown ? GUI::EVENT_TYPE_KEYDOWN : GUI::EVENT_TYPE_KEYUP;
				
				pEventData = (GUI::SKeyEvent *)MemAlloc(sizeof(*pEventData));*/
				ASSERT(0);

				//map the keycode
				//eventData.keyCode =
				
				//translate keystroke to char
				/*pEventData->codePoint = MapVirtualKey(ir.Event.KeyEvent.wVirtualKeyCode, MAPVK_VK_TO_CHAR);
				if(!(ir.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED))
					pEventData->codePoint = ToLowercase(pEventData->codePoint);
				
				char key[2];
				key[0] = (char)pEventData->codePoint;
				key[1] = 0;
				::MessageBox(NULL, key, "Unhandeled key event", 0);
				
				refEvent.Set(eventType, pEventData);*/
			}
			return true;
		case WINDOW_BUFFER_SIZE_EVENT:
			{
				refEvent.Set(EVENT_TYPE_RESIZED, nullptr);
			}
			return true;
		}
	}
	
	return false;
}

void CConsoleRenderTarget::Present()
{
	void *pTmp;

	//present backbuffer
	SetConsoleActiveScreenBuffer(this->pBackBuffer);
	
	//swap buffers
	pTmp = this->pBackBuffer;
	this->pBackBuffer = this->pFrontBuffer;
	this->pFrontBuffer = pTmp;
}

void CConsoleRenderTarget::ResizeView()
{
	uint16 nRows;
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;

	//This method is now safe and well tested for windows 7
	
	//disable scrolling
	csbi.cbSize = sizeof(csbi);
	GetConsoleScreenBufferInfoEx(this->pFrontBuffer, &csbi);
	
	nRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	
	csbi.dwSize.Y = nRows;
	csbi.srWindow.Right++; //bug in SetConsoleScreenBufferInfoEx
	csbi.srWindow.Bottom++; //bug in SetConsoleScreenBufferInfoEx
	
	SetConsoleScreenBufferInfoEx(this->pFrontBuffer, &csbi);
	
	//do the same for the back buffer
	csbi.cbSize = sizeof(csbi);
	GetConsoleScreenBufferInfoEx(this->pBackBuffer, &csbi);
	
	csbi.dwSize.Y = nRows;
	csbi.srWindow.Right++; //bug in SetConsoleScreenBufferInfoEx
	csbi.srWindow.Bottom++; //bug in SetConsoleScreenBufferInfoEx

	SetConsoleScreenBufferInfoEx(this->pBackBuffer, &csbi);
}

void CConsoleRenderTarget::SetBackgroundColor(const CColor &refColor)
{
	this->backgroundColor = refColor;

	SetConsoleTextAttribute(this->pBackBuffer, GetTextAttributes(this->foregroundColor, this->backgroundColor));
}

void CConsoleRenderTarget::SetCursorPos(uint16 x, uint16 y)
{
	COORD coord;

	coord.X = x;
	coord.Y = y;
	
	SetConsoleCursorPosition(this->pFrontBuffer, coord);
}

void CConsoleRenderTarget::SetForegroundColor(const CColor &refColor)
{
	this->foregroundColor = refColor;

	SetConsoleTextAttribute(this->pBackBuffer, GetTextAttributes(this->foregroundColor, this->backgroundColor));
}

void CConsoleRenderTarget::ShowCursor(bool show)
{
	CONSOLE_CURSOR_INFO cci;

	cci.bVisible = show;
	cci.dwSize = 100;
	
	SetConsoleCursorInfo(this->pFrontBuffer, &cci);
	SetConsoleCursorInfo(this->pBackBuffer, &cci);
}