/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
*
* This file is part of Std++.
*
* Std++ is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Std++ is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Std++.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class header
#include <Std++/UI/Controls/PushButton.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
#include <Std++/UI/WidgetContainer.hpp>
#include "../CFullAccessWidget.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/en-us/library/windows/desktop/bb775943(v=vs.85).aspx
*/

//Public methods
/*
 * void PushButton::CreateOSHandle()
-{
-	this->systemHandle = CreateWindowExA(0, WC_BUTTONA, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
-	SetWindowLongPtr((HWND)this->systemHandle, GWLP_USERDATA, (LONG_PTR)this);
-
-	SendMessage((HWND)this->systemHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
-}

 * Size PushButton::GetSizeHint() const
-{
-	SIZE size;
-
-	//TODO: calc min width
-	//TODO: this seems to be working... dont known how it is with different fonts
-
-	Button_GetIdealSize((HWND)this->systemHandle, &size);
-	if(size.cy < 25)
-		size.cy = 25; //aesthetics
-
-	return Size((uint16)size.cx, (uint16)size.cy);
-}
 */
/*
void PushButton::SetText(const String &refText)
{
	SetWindowTextW((HWND)this->backend, (LPCWSTR)refText.ToUTF16().GetRawZeroTerminatedData());
}
*/