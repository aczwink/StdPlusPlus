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
#include "CWinMessageEventQueue.h"
//Global
#include <CommCtrl.h>
//Local
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include "CFullAccessWidget.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Global Variables
bool g_ignoreMessage;
LRESULT g_messageResult;

//Private functions
void CWinMessageEventQueue::DispatchControlEvent(Widget &refWidget, UINT notificationCode)
{
	switch(notificationCode)
	{
	case BN_CLICKED:
		{
			if(IS_INSTANCE_OF(&refWidget, PushButton))
			{
				PushButton &refButton = (PushButton &)refWidget;

				refButton.onActivatedHandler();
			}
			else if(IS_INSTANCE_OF(&refWidget, CheckBox))
			{
				CheckBox &refCheckBox = (CheckBox &)refWidget;

				EventQueue::DispatchToggledEvent(refCheckBox);
			}
		}
		break;
	case CBN_SELCHANGE:
		{
			ComboBox &refDropDown = (ComboBox &)refWidget;

			EventQueue::DispatchSelectionChangedEvent(refDropDown);
		}
		break;
	}
}

void CWinMessageEventQueue::DispatchNotificationEvent(Widget &refWidget, const NMHDR &refNmHdr)
{
	switch(refNmHdr.code)
	{
	case TVN_SELCHANGEDW:
		{
			TreeView &refTreeView = (TreeView &)refWidget;

			EventQueue::DispatchSelectionChangedEvent(refTreeView);
		}
		break;
	}
}

bool CWinMessageEventQueue::DispatchEvent(Widget &refWidget, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_ignoreMessage = false;

	Window &refWnd = (Window &)refWidget;

	switch(message)
	{
	case WM_DESTROY:
		{
			//only forward to windows
			if(dynamic_cast<Window *>(&refWidget))
			{
				EventQueue::DispatchDestroyEvent(refWnd);
				g_messageResult = 0;
			}
		}
		break;
	case WM_SIZE:
		{
			EventQueue::DispatchResizedEvent(refWnd);
			g_messageResult = 0;
		}
		break;
	case WM_PAINT:
		{
			//very ugly to check this here like this
			if(dynamic_cast<RenderTargetWidget *>(&refWidget))
			{
				RECT rcUpdate;

				//validate the update region (it is important that this is done before calling the event handler because user might want to redraw in paint handler!!!)
				GetUpdateRect(GET_HWND(&refWidget), &rcUpdate, false);
				ValidateRect(GET_HWND(&refWidget), &rcUpdate);
			}

			EventQueue::DispatchPaintEvent(refWidget);
			g_messageResult = 0;
		}
		break;
	case WM_CLOSE:
		{
			EventQueue::DispatchCloseEvent(refWnd);
			g_messageResult = 0;
		}
		break;
	case WM_NOTIFY:
		{
			const NMHDR *const& refpNmHdr = (NMHDR *)lParam;
			Widget &refWidget = *(Widget *)GetWindowLongPtr((HWND)refpNmHdr->hwndFrom, GWLP_USERDATA);

			DispatchNotificationEvent(refWidget, *refpNmHdr);
		}
		break;
	case WM_COMMAND:
		{
			g_messageResult = 0;

			if(lParam)
			{
				DispatchControlEvent(*(Widget *)GetWindowLongPtr((HWND)lParam, GWLP_USERDATA), HIWORD(wParam));
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

//Public functions
LRESULT CALLBACK CWinMessageEventQueue::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Widget *pWidget;

	if(message == WM_NCCREATE)
	{
		pWidget = (Widget *)((LPCREATESTRUCT)lParam)->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWidget);
	}
	else
	{
		pWidget = (Widget *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if(pWidget && CWinMessageEventQueue::DispatchEvent(*pWidget, message, wParam, lParam))
		return g_messageResult;

	return DefWindowProcW(hWnd, message, wParam, lParam);
}