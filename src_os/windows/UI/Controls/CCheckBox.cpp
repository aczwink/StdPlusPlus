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
#include <ACStdLib/UI/Controls/CheckBox.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-16/UTF16String.hpp>
#include <ACStdLib/UI/WidgetContainer.hpp>
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/de-de/library/windows/desktop/bb775943(v=vs.85).aspx
*/

//Destructor
CheckBox::~CheckBox()
{
}

//Public methods
Size CheckBox::GetSizeHint() const
{
	HFONT hFont;
	HDC hDC;
	SIZE s;
	Size size;
	UTF16String text;

	size.width = GetSystemMetrics(SM_CXMENUCHECK);
	size.height = GetSystemMetrics(SM_CYMENUCHECK);

	hDC = GetDC((HWND)this->systemHandle);
	hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	text = this->GetText().GetUTF16();

	SelectObject(hDC, hFont);
	GetTextExtentPoint32W(hDC, (LPCWSTR)text.GetC_Str(), text.GetLength(), &s);

	ReleaseDC((HWND)this->systemHandle, hDC);

	size.width += (uint16)s.cx;
	size.height += (uint16)s.cy;

	return size;
}

String CheckBox::GetText() const
{
	uint16 buffer[1000]; //should be sufficient

						 //length = SendMessageW((HWND)this->pOSHandle, WM_GETTEXTLENGTH, 0, 0);
	SendMessageW((HWND)this->systemHandle, WM_GETTEXT, sizeof(buffer) / sizeof(buffer[0]), (LPARAM)buffer);

	return UTF16String(buffer);
}

bool CheckBox::IsChecked() const
{
	return SendMessage((HWND)this->systemHandle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void CheckBox::SetText(const String &refText)
{
	const UTF16String &text = refText.GetUTF16();

	SetWindowTextW((HWND)this->systemHandle, (LPCWSTR)text.GetC_Str());
}

//Private methods
void CheckBox::System_CreateHandle()
{
	this->systemHandle = CreateWindowExA(0, WC_BUTTONA, nullptr, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->systemHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->systemHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}