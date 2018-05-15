#include <Std++/_Backends/WindowBackendType.hpp>

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
#include "CommCtrlWindowBackend.hpp"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <Std++/UI/Widget.hpp>
#include <Std++/UI/WidgetContainer.hpp>
#include "Definitions.h"
//Namespaces
using namespace _stdpp;

/*
WinAPI Documentation:
	PushButon: https://msdn.microsoft.com/en-us/library/windows/desktop/bb775943(v=vs.85).aspx
*/

//Constructor
CommCtrlWindowBackend::CommCtrlWindowBackend(UIBackend *uiBackend, _stdpp::WindowBackendType type, Widget *widget, HWND hParent) : WindowBackend(uiBackend, type, widget)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
    switch(type)
    {
	case WindowBackendType::CheckBox:
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	break;
	case WindowBackendType::ListView:
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	break;
		case WindowBackendType::PushButton:
		{
			this->hWnd = CreateWindowExW(0, WC_BUTTONW, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
		}
		break;
		case WindowBackendType::TextEdit:
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
		break;
        case WindowBackendType::Window:
            {
                this->hWnd = CreateWindowExW(0, STDPLUSPLUS_WIN_WNDCLASS, nullptr, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0, 0, 0, hParent, NULL, hInstance, widget);
            }
            break;
        default:
            NOT_IMPLEMENTED_ERROR; //TODO: implement me
    }

	if(this->type != WindowBackendType::Window)
		SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)this);

	this->SendMessage(WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

//Destructor
CommCtrlWindowBackend::~CommCtrlWindowBackend()
{
    DestroyWindow(this->hWnd);
}

//Public methods
void CommCtrlWindowBackend::ClearView() const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

WindowBackend *CommCtrlWindowBackend::CreateChildBackend(WindowBackendType type, Widget *widget) const
{
	return new CommCtrlWindowBackend(this->GetUIBackend(), type, widget, this->hWnd);
}

Rect CommCtrlWindowBackend::GetChildrenRect() const
{
	RECT rc;
	GetClientRect(this->hWnd, &rc);

	return {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
}

Size CommCtrlWindowBackend::GetSize() const
{
    RECT rc;
    GetWindowRect(this->hWnd, &rc);

    return {static_cast<uint16>(rc.right - rc.left), static_cast<uint16>(rc.bottom - rc.top)};
}

Size CommCtrlWindowBackend::GetSizeHint() const
{
	switch (type)
	{
		case WindowBackendType::PushButton:
		{			
			//TODO: calc min width
			//TODO: this seems to be working... dont known how it is with different fonts

			SIZE size{};
			this->SendMessage(BCM_GETIDEALSIZE, 0, (LPARAM)&size);
			if (size.cy < 25)
				size.cy = 25; //aesthetics
			return Size((uint16)size.cx, (uint16)size.cy);
		}
		break;
	}

    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return Size();
}

void CommCtrlWindowBackend::Maximize()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::Paint()
{
	switch(this->type)
	{
		case WindowBackendType::Window:
		{
			HBRUSH hBrush;
			PAINTSTRUCT ps;

			BeginPaint(this->hWnd, &ps);
			hBrush = GetSysColorBrush(COLOR_MENU); //stupid winapi.. this should be COLOR_WINDOW... it seems that microsoft doesn't understand its own api

			FillRect(ps.hdc, &ps.rcPaint, hBrush);

			EndPaint(this->hWnd, &ps);
		}
		break;
	}
}

void CommCtrlWindowBackend::Repaint()
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path CommCtrlWindowBackend::SelectExistingDirectory(const StdPlusPlus::String &title, const StdPlusPlus::Function<bool(StdPlusPlus::Path &)> callback) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return Path();
}

void CommCtrlWindowBackend::SetBounds(const Rect &area)
{
	Point translated;
	if(this->type == WindowBackendType::Window)
		translated = area.origin;
	else
		//we got called just after this->bounds = area happened
		translated = this->widget->TranslateToOwnerCoords(Point()); //area.origin is relative to the parent!
    SetWindowPos(this->hWnd, HWND_TOP, translated.x, translated.y, area.width(), area.height(), SWP_NOZORDER);
}

void CommCtrlWindowBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetEnabled(bool enable) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetHint(const StdPlusPlus::String &text) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetMaximum(uint32 max) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetMinimum(uint32 min) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetPosition(uint32 pos) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetText(const String &text)
{
    SendMessageW(this->hWnd, WM_SETTEXT, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
}

void CommCtrlWindowBackend::SetValue(int32 value) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::Show(bool visible)
{
    ShowWindow(this->hWnd, visible ? SW_SHOW : SW_HIDE);
}

void CommCtrlWindowBackend::ShowInformationBox(const StdPlusPlus::String &title, const StdPlusPlus::String &message) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}