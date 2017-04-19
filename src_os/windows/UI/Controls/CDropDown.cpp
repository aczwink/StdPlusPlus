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
#include "../../../../headers/UI/Controls/CDropDown.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/en-us/library/windows/desktop/bb775792(v=vs.85).aspx
*/

//Constructor
CDropDown::CDropDown(AWidgetContainer *pParent) : AWidget(pParent)
{
	this->pOSHandle = CreateWindowExA(0, WC_COMBOBOXA, nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

	this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Fixed);
}

//Public methods
uint32 CDropDown::AddItem(const CString &refText)
{
	CUTF16String textUTF16;

	textUTF16 = refText.GetUTF16();
	return (uint32)SendMessageW((HWND)this->pOSHandle, CB_ADDSTRING, 0, (LPARAM)textUTF16.GetC_Str());
}

void CDropDown::Clear()
{
	SendMessage((HWND)this->pOSHandle, CB_RESETCONTENT, 0, 0);
}

uint32 CDropDown::GetNumberOfItems() const
{
	return (uint32)SendMessage((HWND)this->pOSHandle, CB_GETCOUNT, 0, 0);
}

uint16 CDropDown::GetSelectedIndex() const
{
	int32 selection;

	selection = SendMessage((HWND)this->pOSHandle, CB_GETCURSEL, 0, 0);
	if(selection == CB_ERR)
		return UINT16_MAX;

	return (uint16)selection;
}

CSize CDropDown::GetSizeHint() const
{
	uint16 x;
	uint32 n, i;
	HDC hDC;
	HFONT hFont;
	wchar_t buffer[1024];
	SIZE size;

	//TODO: this seems to be working... dont known how it is with different fonts
	//do this correctly

	hDC = GetDC((HWND)this->pOSHandle);
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hDC, hFont);

	x = 50; //minimum
	n = this->GetNumberOfItems();
	for(i = 0; i < n; i++)
	{
		SendMessageW((HWND)this->pOSHandle, CB_GETLBTEXT, i, (LPARAM)buffer);
		GetTextExtentPoint32W(hDC, buffer, GetStringLength(buffer), &size);
		if(size.cx > x)
			x = (uint16)size.cx;
	}

	ReleaseDC((HWND)this->pOSHandle, hDC);

	x += 10; //add some spacing left and right

	return CSize(x, 21);
}

void CDropDown::Select(int32 index)
{
	SendMessage((HWND)this->pOSHandle, CB_SETCURSEL, index, 0);
}

void CDropDown::SetHint(const CString &refText)
{
	CUTF16String textUTF16;

	textUTF16 = refText.GetUTF16();
	SendMessageW((HWND)this->pOSHandle, CB_SETCUEBANNER, 0, (LPARAM)textUTF16.GetC_Str());
}