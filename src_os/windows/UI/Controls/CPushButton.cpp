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
#include "../../../../headers/UI/Controls/CPushButton.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/en-us/library/windows/desktop/bb775943(v=vs.85).aspx
*/

//Private methods
void PushButton::CreateOSHandle()
{
	this->pOSHandle = CreateWindowExA(0, WC_BUTTONA, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

//Public methods
CSize CPushButton::GetSizeHint() const
{
	SIZE size;

	//TODO: calc min width
	//TODO: this seems to be working... dont known how it is with different fonts

	Button_GetIdealSize((HWND)this->pOSHandle, &size);
	if(size.cy < 25)
		size.cy = 25; //aesthetics

	return CSize((uint16)size.cx, (uint16)size.cy);
}

void CPushButton::SetText(const CUTF8String &refText)
{
	CUTF16String text(refText);

	SetWindowTextW((HWND)this->pOSHandle, (LPCWSTR)text.GetC_Str());
}