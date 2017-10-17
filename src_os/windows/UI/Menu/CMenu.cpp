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
#include <ACStdLib/UI/Menu/CMenu.h>
//Global
#include <Windows.h>
//Local
#include <ACStdLib/UI/CAction.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
CMenu::CMenu(const String &refText)
{
	this->pOSHandle = CreateMenu();
}

//Private methods
void CMenu::AppendItemOS(CActionEntry *pEntry)
{
	MENUITEMINFOW itemInfo;

	//append item
	AppendMenuW((HMENU)this->pOSHandle, MF_STRING, (UINT_PTR)pEntry, (LPCWSTR)pEntry->refAction.text.GetUTF16().GetC_Str());

	//set item info
	itemInfo.cbSize = sizeof(itemInfo);
	itemInfo.fMask = MIIM_DATA;
	itemInfo.dwItemData = (ULONG_PTR)pEntry;

	SetMenuItemInfoW((HMENU)this->pOSHandle, this->menuEntries.GetNumberOfElements() - 1, TRUE, &itemInfo);
}

void CMenu::DestructOS()
{
	DestroyMenu((HMENU)this->pOSHandle);
}

//Public methods
void CMenu::AppendSeperator()
{
	this->menuEntries.Push(nullptr);

	AppendMenuA((HMENU)this->pOSHandle, MF_SEPARATOR, 0, nullptr);
}

void CMenu::AppendSubMenu(CMenu *pSubMenu)
{
	/*
	AppendMenuW((HMENU)this->pOSHandle, MF_POPUP | MF_STRING, (UINT_PTR)pSubMenu->pOSHandle, (LPCWSTR)pSubMenu->GetText().GetC_Str());

	this->menuEntries.Push(pSubMenu);
	*/
	NOT_IMPLEMENTED_ERROR;
}