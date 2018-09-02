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
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/UI/Views/View.hpp>
#include <Std++/UI/Widget.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "CommCtrlMenuBarBackend.hpp"
#include "CommCtrlContainerBackend.hpp"
#include "../Imports.h"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;

/*
WinAPI Documentation:
	CheckBox: https://msdn.microsoft.com/de-de/library/windows/desktop/bb775943(v=vs.85).aspx
	Label: https://msdn.microsoft.com/en-us/library/windows/desktop/bb760769(v=vs.85).aspx
	ListView: https://msdn.microsoft.com/en-us/library/windows/desktop/bb775146(v=vs.85).aspx
	SpinBox: https://msdn.microsoft.com/en-us/library/windows/desktop/bb759880(v=vs.85).aspx
	TextEdit: https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx

	this->hWndReal = nullptr;

    switch(type)
    {
	case WindowBackendType::CheckBox:
	{
		this->hWnd = CreateWindowExW(0, WC_BUTTONW, nullptr, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
	}
	break;
	case WindowBackendType::Label:
	{
		this->hWnd = CreateWindowExW(0, WC_STATICW, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
	}
	break;
	case WindowBackendType::ListView:
	{
		this->hWnd = CreateWindowExW(0, WC_LISTBOXW, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
	}
	break;
		case WindowBackendType::SpinBox:
		{
			this->hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, nullptr, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
			this->hWndReal = CreateWindowExW(0, UPDOWN_CLASSW, nullptr, WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK | UDS_SETBUDDYINT, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
			SendMessageW(this->hWndReal, UDM_SETBUDDY, (WPARAM)this->hWnd, 0);
		}
		break;
		case WindowBackendType::TextEdit:
		{
			this->hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_EDITW, nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE, 0, 0, 0, 0, hParent, nullptr, hInstance, nullptr);
		}
		break;
    }*/

//Public methods
void CommCtrlWindowBackend::AddChild(Widget *widget)
{
	CommCtrlWidgetBackend *commCtrlWidgetBackend = dynamic_cast<CommCtrlWidgetBackend *>(widget->_GetBackend());
	commCtrlWidgetBackend->Reparent(this);
}

CompositeWidget *CommCtrlWindowBackend::CreateContentArea()
{
	return new CommCtrlContainer;
}

RectD CommCtrlWindowBackend::GetContentAreaBounds() const
{
	RECT rcWindow;
	GetWindowRect(this->Get_HWND_ReadOnly(), &rcWindow);

	RECT rcClient;
	GetClientRect(this->Get_HWND_ReadOnly(), &rcClient);

	POINT origin{ 0, rcClient.bottom };
	ClientToScreen(this->Get_HWND_ReadOnly(), &origin);

	return RectD(origin.x - rcWindow.left, rcWindow.bottom - origin.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
}

/*
Rect CommCtrlWindowBackend::GetChildrenRect() const
{
	RECT rc;
	GetClientRect(this->hWnd, &rc);
	Rect result = {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};

	return result;
}*/

uint32 CommCtrlWindowBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint32();
}

/*void CommCtrlWindowBackend::GetRange(int32 &min, int32 &max)
{
	SendMessageW(this->hWndReal, UDM_GETRANGE32, (WPARAM)&min, (LPARAM)&max);
}*/

Math::SizeD _stdxx_::CommCtrlWindowBackend::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Math::SizeD();
}

Widget &CommCtrlWindowBackend::GetWidget()
{
	return *this->window;
}

const Widget &CommCtrlWindowBackend::GetWidget() const
{
	return *this->window;
}

/*Size CommCtrlWindowBackend::GetSize() const
{
    RECT rc;
    GetWindowRect(this->hWnd, &rc);

    return {static_cast<uint16>(rc.right - rc.left), static_cast<uint16>(rc.bottom - rc.top)};
}

Size CommCtrlWindowBackend::GetSizeHint() const
{
	switch (type)
	{
	case WindowBackendType::CheckBox:
	{
		Size checkSize;
		checkSize.width = GetSystemMetrics(SM_CXMENUCHECK);
		checkSize.height = GetSystemMetrics(SM_CYMENUCHECK);
		return checkSize + this->GetTextExtents();
	}
		break;
	case WindowBackendType::ComboBox:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		break;
	case WindowBackendType::ListView:
		return Size(); //no idea...
		break;
		case WindowBackendType::RadioButton:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case WindowBackendType::RenderTarget:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case WindowBackendType::SearchBox:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case WindowBackendType::Slider:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case WindowBackendType::SpinBox:
		{
			//TODO: min width
			//TODO: this seems to be working... dont known how it is with different fonts

			return Size(40, 26);
		}
		break;
		case WindowBackendType::TreeView:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case WindowBackendType::Label:
		case WindowBackendType::TextEdit:
		case WindowBackendType::Window: //at least the title should be displayed
		{
			return this->GetTextExtents();
		}
		break;
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return Size();
}

int32 CommCtrlWindowBackend::GetValue() const
{
	BOOL valid;
	int32 value = SendMessageW(this->hWndReal, UDM_GETPOS32, 0, (LPARAM)&valid);
	if (valid == FALSE)
		return 0;
	return value;
}*/

void CommCtrlWindowBackend::IgnoreEvent()
{
	extern bool g_ignoreMessage;
	g_ignoreMessage = true;
}

/*bool CommCtrlWindowBackend::IsChecked() const
{
	return this->SendMessage(BM_GETCHECK, 0, 0) == BST_CHECKED;
}*/

void CommCtrlWindowBackend::Maximize()
{
	this->maximizeWindow = true;
}

void CommCtrlWindowBackend::PrePaint()
{
	PAINTSTRUCT ps;
	BeginPaint(this->GetHWND(), &ps);

	HBRUSH hBrush = GetSysColorBrush(COLOR_MENU); //stupid winapi.. this should be COLOR_WINDOW... it seems that microsoft doesn't understand its own api
	FillRect(ps.hdc, &ps.rcPaint, hBrush);

	EndPaint(this->GetHWND(), &ps);
}

void CommCtrlWindowBackend::Repaint()
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlWindowBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

/*void CommCtrlWindowBackend::ResetView() const
{
	TreeController *controller = ((View *)this->widget)->GetController();

	switch (this->type)
	{
	case WindowBackendType::ListView:
	{
		this->SendMessage(LB_RESETCONTENT, 0, 0);

		if (controller)
		{
			String text;

			uint32 nItems = controller->GetNumberOfChildren();
			this->SendMessage(LB_INITSTORAGE, nItems, 50);
			for (uint32 i = 0; i < nItems; i++)
			{
				text = controller->GetText(controller->GetChildIndex(i, 0));
				SendMessageW(this->hWnd, LB_ADDSTRING, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
				SendMessageW(this->hWnd, LB_SETITEMDATA, i, i);
			}
		}
	}
		break;
	default:
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}*/

void CommCtrlWindowBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path CommCtrlWindowBackend::SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(StdXX::Path &)> callback) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return Path();
}

void CommCtrlWindowBackend::SetBounds(const RectD &bounds)
{
	this->SetRect(bounds);
}

/*void CommCtrlWindowBackend::SetBounds(const Rect &area)
{
	switch (this->type)
	{
	case WindowBackendType::SpinBox:
	{
		int w2 = 20;
		int w1 = area.width() - w2;
		if (area.width() <= w2)
			w1 = w2 = area.width() / 2;
		SetWindowPos(this->hWnd, HWND_TOP, translated.x, translated.y, w1, area.height(), SWP_NOZORDER);
		SetWindowPos(this->hWndReal, HWND_TOP, translated.x + w1, translated.y, w2, area.height(), SWP_NOZORDER);
	}
		break;
	}
}*/

void CommCtrlWindowBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me next line
	//SendMessageA(this->hWnd, EM_SETREADONLY, !enable, 0);
}

void CommCtrlWindowBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me next line
	//EnableWindow(this->hWnd, enable);
}

void CommCtrlWindowBackend::SetHint(const StdXX::String &text) const
{
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlWindowBackend::SetMenuBar(MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{	
	CommCtrlMenuBarBackend *commCtrlMenuBarBackend = (CommCtrlMenuBarBackend *)menuBarBackend;
	::SetMenu(this->GetHWND(), commCtrlMenuBarBackend->GetHandle());

	//bind hWnd to hMenu
	MENUINFO mi;
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;
	mi.dwMenuData = (ULONG_PTR)this;
	
	SetMenuInfo(commCtrlMenuBarBackend->GetHandle(), &mi);
}

void CommCtrlWindowBackend::SetTitle(const String & title)
{
	this->SetText(title);
}

void CommCtrlWindowBackend::Show(bool visible)
{
	if (visible)
	{
		//was the window ever visible?
		if (this->showFirstTime)
		{
			ShowWindow(this->GetHWND(), SW_SHOWNORMAL);
			this->showFirstTime = false;
		}
		else
		{
			ShowWindow(this->GetHWND(), SW_SHOW);
		}

		//maximize?
		if (this->maximizeWindow)
		{
			ShowWindow(this->GetHWND(), SW_MAXIMIZE);
			this->maximizeWindow = false;
		}
	}
	else
	{
		ShowWindow(this->GetHWND(), SW_HIDE);
	}
}

void _stdxx_::CommCtrlWindowBackend::ShowInformationBox(const StdXX::String & title, const StdXX::String & message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlWindowBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

/*void CommCtrlWindowBackend::SetRange(int32 min, int32 max)
{
	SendMessageW(this->hWndReal, UDM_SETRANGE32, min, max);
	//update pos always because of redraw
	this->SetValue(this->GetValue());
}

void CommCtrlWindowBackend::SetValue(int32 value)
{
	SendMessageW(this->hWndReal, UDM_SETPOS32, 0, value);
}

void CommCtrlWindowBackend::Show(bool visible)
{
	if (this->type == WindowBackendType::Window)
	{
		Size size = this->widget->GetSizeHint();

		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = size.width;
		rc.bottom = size.height;
		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

		SetWindowPos(this->hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
	}
}*/