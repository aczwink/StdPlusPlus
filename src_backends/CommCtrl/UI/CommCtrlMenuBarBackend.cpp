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
#include "CommCtrlMenuBarBackend.hpp"
//Local
#include "CommCtrlMenuBackend.hpp"
#include "CommCtrlWindowBackend.hpp"
//Namespaces
using namespace _stdxx_;

//Constructor
CommCtrlMenuBarBackend::CommCtrlMenuBarBackend(MenuBar *menuBar)
{
	this->hMenu = CreateMenu();

	//set that menu should inform via WM_MENUCOMMAND, because WM_COMMAND only gives an ID but not a handle
	MENUINFO menuInfo;
	menuInfo.cbSize = sizeof(menuInfo);
	menuInfo.fMask = MIM_STYLE;
	GetMenuInfo(this->hMenu, &menuInfo);
	menuInfo.dwStyle |= MNS_NOTIFYBYPOS;
	SetMenuInfo(this->hMenu, &menuInfo);
}

//Destructor
CommCtrlMenuBarBackend::~CommCtrlMenuBarBackend()
{
	DestroyMenu(this->hMenu);
}

//Public methods
void CommCtrlMenuBarBackend::AppendMenu(Menu *menu)
{
	const CommCtrlMenuBackend *menuBackend = (CommCtrlMenuBackend *)menu->_GetBackend();
	AppendMenuW(this->hMenu, MF_POPUP | MF_STRING, (UINT_PTR)menuBackend->GetHandle(), (LPCWSTR)menu->GetText().ToUTF16().GetRawZeroTerminatedData());

	this->Update();
}

//Private methods
void CommCtrlMenuBarBackend::Update()
{
	MENUINFO mi;

	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;

	GetMenuInfo(hMenu, &mi);

	CommCtrlWindowBackend *wndBackend = (CommCtrlWindowBackend *)mi.dwMenuData;
	DrawMenuBar(wndBackend->GetHandle());
}