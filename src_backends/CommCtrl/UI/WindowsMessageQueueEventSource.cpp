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
#include "WindowsMessageQueueEventSource.hpp"
//Local
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include <Std++/UI/Controls/PushButton.hpp>
#include <Std++/UI/Action.hpp>
#include <Std++/UI/Menu/Menu.hpp>
#include <Std++/UI/Views/ComboBox.hpp>
#include <Std++/UI/Views/TreeView.hpp>
#include "CommCtrlWindowBackend.hpp"
#include "../Imports.h"
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;
using namespace _stdxx_;

//Global variables
bool g_ignoreMessage;
//Local variables
static WindowsMessageQueueEventSource *l_winMsgEvtQueue;
static LRESULT l_messageResult;

//Constructor
WindowsMessageQueueEventSource::WindowsMessageQueueEventSource()
{
	l_winMsgEvtQueue = this;
}

//Public methods
void WindowsMessageQueueEventSource::DispatchPendingEvents()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
			break;

		DispatchMessage(&msg);
	}
}

uint64 WindowsMessageQueueEventSource::GetMaxTimeout() const
{
	return Natural<uint64>::Max();
}

void WindowsMessageQueueEventSource::VisitWaitObjects(const Function<void(_stdxx_::WaitObjHandle, bool)>& visitFunc) const
{
}

//Private methods
void WindowsMessageQueueEventSource::DispatchControlEvent(CommCtrlWidgetBackend &backend, UINT notificationCode)
{
	Widget &widget = backend.GetWidget();
	switch(notificationCode)
	{
		case BN_CLICKED:
		{
			if (IS_INSTANCE_OF(&widget, CheckBox))
			{
				CheckBox &checkBox = dynamic_cast<CheckBox &>(widget);
				this->DispatchToggledEvent(checkBox);
			}
			else if (IS_INSTANCE_OF(&widget, PushButton))
			{
				PushButton &pushButton = dynamic_cast<PushButton &>(widget);
				this->DispatchActivatedEvent(pushButton);
			}
		}
		break;
		case CBN_SELCHANGE: //this is equal to LBN_SELCHANGE
		{
			View &view = (View &)widget;

			this->DispatchSelectionChangedEvent(view);
		}
		break;
	}
}

bool WindowsMessageQueueEventSource::DispatchMessageEvent(CommCtrlWidgetBackend &backend, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_ignoreMessage = false;
	
	Widget &widget = backend.GetWidget();
	Window *window = dynamic_cast<Window *>(&widget);
	
	switch(message)
	{
	case WM_SIZE:
	{
		if (window)
		{ //we forward this only to windows, which will forward the resize to all others
			RECT rcWindow;
			GetWindowRect(hWnd, &rcWindow);

			RectD bounds(rcWindow.left, rcWindow.top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
			widget.SetBounds(bounds);
			l_messageResult = 0;
		}
		else
		{
			return false;
		}
	}
	break;
	case WM_PAINT:
	{
		backend.PrePaint();		
		this->DispatchPaintEvent(widget);
		l_messageResult = 0;
	}
	break;
	case WM_CLOSE:
	{
		this->DispatchCloseEvent(*window);
		l_messageResult = 0;
	}
	break;
		case WM_NOTIFY:
		{
			const NMHDR *const& refpNmHdr = (NMHDR *)lParam;
			Widget &refWidget = *(Widget *)GetWindowLongPtr((HWND)refpNmHdr->hwndFrom, GWLP_USERDATA);

			this->DispatchNotificationEvent(refWidget, *refpNmHdr);
		}
			break;
		case WM_COMMAND:
		{
			l_messageResult = 0;

			if(lParam)
			{
				this->DispatchControlEvent(*(CommCtrlWidgetBackend *)GetWindowLongPtr((HWND)lParam, GWLP_USERDATA), HIWORD(wParam));
			}
			else
			{
				/*
				if(HIGH16(wParam))
				{
				//Accelerator
				return false;
				}
				else
				{
				//Menu
				return false;
				}
				 */
				return false;
			}
		}
		break;
	case WM_MENUCOMMAND:
	{
		MENUITEMINFOW menuItemInfo;
		menuItemInfo.cbSize = sizeof(menuItemInfo);
		menuItemInfo.fMask = MIIM_DATA;
		GetMenuItemInfoW((HMENU)lParam, wParam, TRUE, &menuItemInfo);

		ActionEntry *actionEntry = (ActionEntry *)menuItemInfo.dwItemData;

		if (actionEntry->action->triggeredEvent.IsBound())
			actionEntry->action->triggeredEvent();

		l_messageResult = 0; //THE DOCUMENTATION SAYS NOTHIGN ABOUT THE RETURN VALUE
	}
	break;
	default:
		return false;
	}

	return !g_ignoreMessage;
}

void WindowsMessageQueueEventSource::DispatchNotificationEvent(Widget &refWidget, const NMHDR &refNmHdr)
{
	switch (refNmHdr.code)
	{
	case TVN_SELCHANGEDW:
	{
		TreeView &refTreeView = (TreeView &)refWidget;

		this->DispatchSelectionChangedEvent(refTreeView);
	}
	break;
	}
}

//Class functions
LRESULT CALLBACK WindowsMessageQueueEventSource::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CommCtrlWidgetBackend *backend;
	if(message == WM_NCCREATE)
	{
		backend = (CommCtrlWidgetBackend *)((LPCREATESTRUCT)lParam)->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)backend);
	}
	else
	{
		backend = (CommCtrlWidgetBackend *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if(backend && l_winMsgEvtQueue->DispatchMessageEvent(*backend, hWnd, message, wParam, lParam))
		return l_messageResult;

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

/*
void WinEventQueueBackend::PostQuitEvent()
{
	PostQuitMessage(EXIT_SUCCESS);
}
*/