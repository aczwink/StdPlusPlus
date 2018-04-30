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
//Global
#include <CommCtrl.h>
//Local
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include <Std++/UI/Controls/PushButton.hpp>
#include <Std++/UI/Views/ComboBox.hpp>
#include <Std++/UI/Views/TreeView.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

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

void WindowsMessageQueueEventSource::VisitWaitObjects(const Function<void(_stdpp::WaitObjHandle, bool)>& visitFunc) const
{
}

//Private methods
void WindowsMessageQueueEventSource::DispatchControlEvent(Widget &widget, UINT notificationCode)
{
	switch(notificationCode)
	{
		case BN_CLICKED:
		{
			if(IS_INSTANCE_OF(&widget, PushButton))
			{
				PushButton &refButton = (PushButton &)widget;

				refButton.onActivatedHandler();
			}
			else if(IS_INSTANCE_OF(&widget, CheckBox))
			{
				CheckBox &refCheckBox = (CheckBox &)widget;

				this->DispatchToggledEvent(refCheckBox);
			}
		}
			break;
		case CBN_SELCHANGE:
		{
			ComboBox &refDropDown = (ComboBox &)widget;

			this->DispatchSelectionChangedEvent(refDropDown);
		}
			break;
	}
}

bool WindowsMessageQueueEventSource::DispatchMessageEvent(Widget &widget, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_ignoreMessage = false;

	Window &window = (Window &)widget;

	switch(message)
	{
		case WM_SIZE:
		{
			this->DispatchResizedEvent(window);
			l_messageResult = 0;
		}
			break;
		case WM_PAINT:
		{
			//very ugly to check this here like this
			if(dynamic_cast<RenderTargetWidget *>(&widget))
			{
				RECT rcUpdate;

				//validate the update region (it is important that this is done before calling the event handler because user might want to redraw in paint handler!!!)
				NOT_IMPLEMENTED_ERROR; //TODO: next line
				//GetUpdateRect(GET_HWND(&widget), &rcUpdate, false);
				//ValidateRect(GET_HWND(&widget), &rcUpdate);
			}

			this->DispatchPaintEvent(widget);
			l_messageResult = 0;
		}
			break;
		case WM_CLOSE:
		{
			this->DispatchCloseEvent(window);
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
				this->DispatchControlEvent(*(Widget *)GetWindowLongPtr((HWND)lParam, GWLP_USERDATA), HIWORD(wParam));
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
			/*
			CMenuItem *pItem;
			MENUITEMINFO menuItemInfo;

			menuItemInfo.cbSize = sizeof(menuItemInfo);
			menuItemInfo.fMask = MIIM_DATA;

			GetMenuItemInfoA((HMENU)lParam, wParam, TRUE, &menuItemInfo);
			pItem = (CMenuItem *)menuItemInfo.dwItemData;

			if(pItem->triggeredEvent)
			pItem->triggeredEvent();

			g_messageResult = 0; //THE DOCUMENTATION SAYS NOTHIGN ABOUT THE RETURN VALUE
			 */
			NOT_IMPLEMENTED_ERROR;
		}
		break;
	default:
		return false;
	}

	return !g_ignoreMessage;
}

//Class functions
LRESULT CALLBACK WindowsMessageQueueEventSource::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Widget *widget;
	if(message == WM_NCCREATE)
	{
		widget = (Widget *)((LPCREATESTRUCT)lParam)->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)widget);
	}
	else
	{
		widget = (Widget *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if(widget && l_winMsgEvtQueue->DispatchMessageEvent(*widget, message, wParam, lParam))
		return l_messageResult;

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

void WindowsMessageQueueEventSource::DispatchNotificationEvent(Widget &refWidget, const NMHDR &refNmHdr)
{
	switch(refNmHdr.code)
	{
		case TVN_SELCHANGEDW:
		{
			TreeView &refTreeView = (TreeView &)refWidget;

			this->DispatchSelectionChangedEvent(refTreeView);
		}
			break;
	}
}

/*
void WinEventQueueBackend::PostQuitEvent()
{
	PostQuitMessage(EXIT_SUCCESS);
}
*/