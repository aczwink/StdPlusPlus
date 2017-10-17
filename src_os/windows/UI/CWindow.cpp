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
#include <ACStdLib/UI/Window.hpp>
//Global
#include <Windows.h>
//Local
#include <ACStdLib/Containers/Strings/UTF-16/UTF16String.hpp>
#include <ACStdLib/UI/Menu/CMenuBar.h>
#include "../../../src/UI/Render Targets/IRenderTarget.h"
#include "CDropTarget.h"
#include "Definitions.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Eventhandlers
void CWindow::OnPaint()
{
	if(this->pOSHandle)
	{
		HBRUSH hBrush;
		PAINTSTRUCT ps;

		BeginPaint((HWND)this->pOSHandle, &ps);
		hBrush = GetSysColorBrush(COLOR_MENU); //stupid winapi.. this should be COLOR_WINDOW... it seems that microsoft doesn't understand its own api

		FillRect(ps.hdc, &ps.rcPaint, hBrush);

		EndPaint((HWND)this->pOSHandle, &ps);
	}
	else
	{
		AWidgetContainer::OnPaint();

		GetRenderTarget(this->GetRenderMode()).Present();
	}
}

//Private methods
void CWindow::CreateOSWindow(const CRect &refRect)
{
    this->pOSHandle = (void *)CreateWindowExW(0, ACSTDLIB_WIN_WNDCLASS, nullptr, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, refRect.x(), refRect.y(), refRect.width(), refRect.height(), NULL, NULL, GetModuleHandle(NULL), this);
}

void CWindow::DestroyOSWindow()
{
	DestroyWindow((HWND)this->pOSHandle);
}

void CWindow::MenuBarChangeOS()
{
	MENUINFO mi;

	::SetMenu((HWND)this->pOSHandle, (HMENU)this->pMenuBar->pOSHandle);

	//bind hWnd to hMenu
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;
	mi.dwMenuData = (ULONG_PTR)this->pOSHandle;

	SetMenuInfo((HMENU)this->pMenuBar->pOSHandle, &mi);
}

//Public methods
void CWindow::EnableDrop()
{
	if(!this->pOSDropTarget)
	{
		this->pOSDropTarget = new CDropTarget(this);
		RegisterDragDrop((HWND)this->pOSHandle, this->pOSDropTarget);
	}
}

void CWindow::Maximize()
{
	ShowWindow((HWND)this->pOSHandle, SW_MAXIMIZE);
}

void CWindow::SetTitle(const CUTF8String &refTitle)
{
	CUTF16String titleUTF16(refTitle);

	SendMessageW((HWND)this->pOSHandle, WM_SETTEXT, 0, (LPARAM)titleUTF16.GetC_Str());
}

void CWindow::ShowErrorBox(const CString &refTitle, const CString &refMessage)
{
	CUTF16String title, message;

	title = refTitle.GetUTF16();
	message = refMessage.GetUTF16();

	MessageBoxW((HWND)this->pOSHandle, (LPCWSTR)message.GetC_Str(), (LPCWSTR)title.GetC_Str(), MB_OK | MB_ICONERROR);
}

void CWindow::ShowInformationBox(const CString &refTitle, const CString &refMessage)
{
	CUTF16String title, message;

	title = refTitle.GetUTF16();
	message = refMessage.GetUTF16();

	MessageBoxW((HWND)this->pOSHandle, (LPCWSTR)message.GetC_Str(), (LPCWSTR)title.GetC_Str(), MB_OK | MB_ICONINFORMATION);
}