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
#include <ACStdLib/UI/Views/ComboBox.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <ACStdLib/UI/Controllers/TreeController.hpp>
#include <ACStdLib/UI/WidgetContainer.hpp>
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/en-us/library/windows/desktop/bb775792(v=vs.85).aspx
*/

//Destructor
ComboBox::~ComboBox()
{
}

//Public methods
Size ComboBox::GetSizeHint() const
{
	uint16 x;
	uint32 n, i;
	HDC hDC;
	HFONT hFont;
	wchar_t buffer[1024];
	SIZE size;

	//TODO: this seems to be working... dont known how it is with different fonts
	//do this correctly

	hDC = GetDC((HWND)this->systemHandle);
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hDC, hFont);

	x = 50; //minimum
	n = this->controller->GetNumberOfChildren(); //number of items
	for(i = 0; i < n; i++)
	{
		SendMessageW((HWND)this->systemHandle, CB_GETLBTEXT, i, (LPARAM)buffer);
		GetTextExtentPoint32W(hDC, buffer, GetStringLength(buffer), &size);
		if(size.cx > x)
			x = (uint16)size.cx;
	}

	ReleaseDC((HWND)this->systemHandle, hDC);

	x += 10; //add some spacing left and right

	return Size(x, 21);
}

//Private methods
void ComboBox::Backend_Create()
{
	this->systemHandle = CreateWindowExA(0, WC_COMBOBOXA, nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->systemHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->systemHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

//Event handlers
void ComboBox::OnModelChanged()
{
	NOT_IMPLEMENTED_ERROR;
}

void ComboBox::OnSelectionChanged()
{
	NOT_IMPLEMENTED_ERROR;
}


/*
OLD:
*/

/*
void ComboBox::Select(int32 index)
{
	SendMessage((HWND)this->systemHandle, CB_SETCURSEL, index, 0);
}

void ComboBox::SetHint(const String &refText)
{
	UTF16String textUTF16;

	textUTF16 = refText.GetUTF16();
	SendMessageW((HWND)this->systemHandle, CB_SETCUEBANNER, 0, (LPARAM)textUTF16.GetC_Str());
}
*/

/*
uint32 ComboBox::AddItem(const String &refText)
{
UTF16String textUTF16;

textUTF16 = refText.GetUTF16();
return (uint32)SendMessageW((HWND)this->systemHandle, CB_ADDSTRING, 0, (LPARAM)textUTF16.GetC_Str());
}

void ComboBox::Clear()
{
SendMessage((HWND)this->systemHandle, CB_RESETCONTENT, 0, 0);
}

uint32 ComboBox::GetNumberOfItems() const
{
return (uint32)SendMessage((HWND)this->systemHandle, CB_GETCOUNT, 0, 0);
}

uint16 ComboBox::GetSelectedIndex() const
{
int32 selection;

selection = SendMessage((HWND)this->systemHandle, CB_GETCURSEL, 0, 0);
if(selection == CB_ERR)
return UINT16_MAX;

return (uint16)selection;
}
*/