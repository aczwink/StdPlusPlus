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
#include <Std++/UI/Window.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
#include <Std++/UI/Menu/CMenuBar.h>
#include "../../../src/UI/Render Targets/IRenderTarget.h"
#include "CDropTarget.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Private methods
void Window::MenuBarChangeOS()
{
	MENUINFO mi;

	::SetMenu((HWND)this->backend, (HMENU)this->pMenuBar->pOSHandle);

	//bind hWnd to hMenu
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;
	mi.dwMenuData = (ULONG_PTR)this->backend;

	SetMenuInfo((HMENU)this->pMenuBar->pOSHandle, &mi);
}

//Public methods
void Window::EnableDrop()
{
	if(!this->pOSDropTarget)
	{
		this->pOSDropTarget = new CDropTarget(this);
		RegisterDragDrop((HWND)this->backend, this->pOSDropTarget);
	}
}

/* OLD STUFF:
void Window::Maximize()
{
ShowWindow((HWND)this->backend, SW_MAXIMIZE);
}

void Window::ShowErrorBox(const String &refTitle, const String &refMessage)
{
	UTF16String title, message;

	title = refTitle.GetUTF16();
	message = refMessage.GetUTF16();

	MessageBoxW((HWND)this->systemHandle, (LPCWSTR)message.GetC_Str(), (LPCWSTR)title.GetC_Str(), MB_OK | MB_ICONERROR);
}
*/
/*
void Window::ShowInformationBox(const String &refTitle, const String &refMessage)
{
	MessageBoxW((HWND)this->backend, (LPCWSTR)refMessage.ToUTF16().GetRawZeroTerminatedData(), (LPCWSTR)refTitle.ToUTF16().GetRawZeroTerminatedData(), MB_OK | MB_ICONINFORMATION);
}
*/