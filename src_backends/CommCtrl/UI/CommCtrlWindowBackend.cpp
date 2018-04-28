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
//Local
#include <Std++/UI/Widget.hpp>
#include "Definitions.h"
//Namespaces
using namespace _stdpp;

//Constructor
CommCtrlWindowBackend::CommCtrlWindowBackend(UIBackend *uiBackend, _stdpp::WindowBackendType type, Widget *widget)
        : type(type),
		  widget(widget)
{
    switch(type)
    {
        case WindowBackendType::Window:
            {
                this->hWnd = CreateWindowExW(0, STDPLUSPLUS_WIN_WNDCLASS, nullptr, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), widget);
            }
            break;
        default:
            NOT_IMPLEMENTED_ERROR; //TODO: implement me
    }
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

WindowBackend *CommCtrlWindowBackend::CreateChildBackend(WindowBackendType type, StdPlusPlus::UI::Widget *widget) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

Size CommCtrlWindowBackend::GetSize() const
{
    RECT rc;
    GetWindowRect(this->hWnd, &rc);

    return {static_cast<uint16>(rc.right - rc.left), static_cast<uint16>(rc.bottom - rc.top)};
}

Size CommCtrlWindowBackend::GetSizeHint() const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return Size();
}

UIBackend *CommCtrlWindowBackend::GetUIBackend()
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
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
    Point transformed = this->widget->TransformToWindow(area.origin);
    SetWindowPos(this->hWnd, HWND_TOP, transformed.x, transformed.y, area.width(), area.height(), 0);
}

void CommCtrlWindowBackend::SetEnabled(bool enable) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetHint(const StdPlusPlus::String &text) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetText(const String &text)
{
    SendMessageW(this->hWnd, WM_SETTEXT, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
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