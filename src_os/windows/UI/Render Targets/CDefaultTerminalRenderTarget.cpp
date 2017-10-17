/*
* Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
*
* This file is part of ACStdLib.
*
* ACStdLib is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ACStdLib is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class header
#include "../../../../src/UI/Render Targets/CDefaultTerminalRenderTarget.h"
//Global
#include <Windows.h>
//Local
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#undef THIS //com
#define THIS ((SConsoleInfo *)(this->pOSHandle))

struct SConsoleInfo
{
	HANDLE hFrontBuffer;
	HANDLE hBackBuffer;
};

//Local functions
static void ResizeConsoleScreenBuffer(void *pBuffer)
{
	uint16 nRows;
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;

	//This function is now safe and well tested for windows 7

	//disable scrolling
	csbi.cbSize = sizeof(csbi);
	GetConsoleScreenBufferInfoEx(pBuffer, &csbi);

	nRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	csbi.dwSize.Y = nRows;
	csbi.srWindow.Right++; //bug in SetConsoleScreenBufferInfoEx
	csbi.srWindow.Bottom++; //bug in SetConsoleScreenBufferInfoEx

	SetConsoleScreenBufferInfoEx(pBuffer, &csbi);
}

//Constructor
CDefaultTerminalRenderTarget::CDefaultTerminalRenderTarget()
{
	this->pOSHandle = MemAlloc(sizeof(SConsoleInfo));

	//enable window events
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT);

	//get front buffer
	THIS->hFrontBuffer = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	THIS->hBackBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//disable auto scroll
	SetConsoleMode(THIS->hFrontBuffer, 0);
	SetConsoleMode(THIS->hBackBuffer, 0);

	ResizeConsoleScreenBuffer(THIS->hFrontBuffer);
	ResizeConsoleScreenBuffer(THIS->hBackBuffer);
}

//Destructor
CDefaultTerminalRenderTarget::~CDefaultTerminalRenderTarget()
{
	CloseHandle(THIS->hFrontBuffer);
	CloseHandle(THIS->hBackBuffer);

	MemFree(this->pOSHandle);
}

//Public methods
Size CDefaultTerminalRenderTarget::GetSize() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(THIS->hFrontBuffer, &csbi);

	return Size(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
}

void CDefaultTerminalRenderTarget::OutputCodepointAt(const Point &refPoint, uint32 codePoint)
{
	DWORD numberOfCharsWritten;
	COORD coord;

	coord.X = refPoint.x;
	coord.Y = refPoint.y;

	SetConsoleCursorPosition(THIS->hBackBuffer, coord);
	WriteConsoleW(THIS->hBackBuffer, &codePoint, 1, &numberOfCharsWritten, nullptr);
}

void CDefaultTerminalRenderTarget::Present()
{
	HANDLE hTmp;

	//present backbuffer
	SetConsoleActiveScreenBuffer(THIS->hBackBuffer);

	//swap buffers
	hTmp = THIS->hBackBuffer;
	THIS->hBackBuffer = THIS->hFrontBuffer;
	THIS->hFrontBuffer = hTmp;
}