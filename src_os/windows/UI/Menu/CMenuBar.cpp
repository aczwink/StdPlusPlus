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
#include <Std++/UI/Menu/CMenuBar.h>
//Global
#include <Windows.h>
//Local
#include <Std++/UI/Menu/CMenu.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Definitions
#undef AppendMenu

//Local functions
static void UpdateMenuBar(HMENU hMenu)
{
	MENUINFO mi;

	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;

	GetMenuInfo(hMenu, &mi);

	if(mi.dwMenuData)
		DrawMenuBar((HWND)mi.dwMenuData);
}

//Constructor
CMenuBar::CMenuBar()
{
	MENUINFO menuInfo;

	this->pOSHandle = CreateMenu();

	//set menu info
	menuInfo.cbSize = sizeof(menuInfo);
	menuInfo.fMask = MIM_STYLE;
	GetMenuInfo((HMENU)this->pOSHandle, &menuInfo);
	menuInfo.dwStyle |= MNS_NOTIFYBYPOS;
	SetMenuInfo((HMENU)this->pOSHandle, &menuInfo);
}

//Private methods
void CMenuBar::AppendMenuOS(CMenu *pMenu)
{
	/*
	CUTF16String textUTF16;

	textUTF16 = pMenu->GetText().GetUTF16();
	AppendMenuW((HMENU)this->pOSHandle, MF_POPUP | MF_STRING, (UINT_PTR)pMenu->pOSHandle, (LPCWSTR)textUTF16.GetC_Str());

	UpdateMenuBar((HMENU)this->pOSHandle);
	*/
	NOT_IMPLEMENTED_ERROR;
}

void CMenuBar::DestroyMenuOS()
{
	DestroyMenu((HMENU)this->pOSHandle);
}