/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "CommCtrlMenuBackend.hpp"
//Local
#include <Std++/UI/Action.hpp>
//Namespaces
using namespace _stdxx_;

//Constructor
CommCtrlMenuBackend::CommCtrlMenuBackend(Menu *menu) : menu(menu)
{
	this->hMenu = CreateMenu();
}

//Destructor
CommCtrlMenuBackend::~CommCtrlMenuBackend()
{
	DestroyMenu(this->hMenu);
}

//Public methods
void CommCtrlMenuBackend::AppendEntry(const StdXX::UI::ActionEntry &actionEntry)
{
	MENUITEMINFOW itemInfo;

	//append item
	AppendMenuW(this->hMenu, MF_STRING, (UINT_PTR)&actionEntry, (LPCWSTR)actionEntry.action->text.ToUTF16().GetRawZeroTerminatedData());

	//set item info
	itemInfo.cbSize = sizeof(itemInfo);
	itemInfo.fMask = MIIM_DATA;
	itemInfo.dwItemData = (ULONG_PTR)&actionEntry;

	SetMenuItemInfoW(this->hMenu, menu->GetEntries().GetNumberOfElements() - 1, TRUE, &itemInfo);
}

void CommCtrlMenuBackend::AppendSeperator()
{
	AppendMenuW(this->hMenu, MF_SEPARATOR, 0, nullptr);
}

void CommCtrlMenuBackend::AppendSubMenu(StdXX::UI::Menu *subMenu)
{
	const CommCtrlMenuBackend *menuBackend = (CommCtrlMenuBackend *)subMenu->_GetBackend();
	AppendMenuW(this->hMenu, MF_POPUP | MF_STRING, (UINT_PTR)menuBackend->GetHandle(), (LPCWSTR)subMenu->GetText().ToUTF16().GetRawZeroTerminatedData());
}

String CommCtrlMenuBackend::GetText() const
{
	return this->text;
}

void CommCtrlMenuBackend::SetText(const String &text)
{
	this->text = text;
}
