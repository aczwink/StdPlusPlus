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
#include <Std++/UI/Controls/CheckBox.hpp>
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
/* OLD STUFF:
//Public methods
	void CheckBox::SetText(const OldString &refText)
	{
	const UTF16String &text = refText.GetUTF16();

	SetWindowTextW((HWND)this->backend, (LPCWSTR)text.GetC_Str());
	}
}*/

OldString CheckBox::GetText() const
{
	uint16 buffer[1000]; //should be sufficient

						 //length = SendMessageW((HWND)this->pOSHandle, WM_GETTEXTLENGTH, 0, 0);
	SendMessageW((HWND)this->backend, WM_GETTEXT, sizeof(buffer) / sizeof(buffer[0]), (LPARAM)buffer);

	return UTF16String(buffer);
}