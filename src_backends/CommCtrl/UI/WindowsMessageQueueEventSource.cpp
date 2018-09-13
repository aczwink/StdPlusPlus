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
#include "CommCtrlRenderTargetWidgetBackend.hpp"
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

void WindowsMessageQueueEventSource::DispatchInputEvent(HWND hWnd, Widget &widget, HRAWINPUT hRawInput)
{
	/*
	source:
	http://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/
	*/

	RAWINPUT input;
	UINT size = sizeof(input);
	GetRawInputData(hRawInput, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER));

	if(input.header.dwType == RIM_TYPEKEYBOARD)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
		/*
		bool isE0, isE1, keyUp;
		UINT virtualKey;
		EKeyCode keyCode;
		extern bool g_keyStates[256];

		isE0 = (input.data.keyboard.Flags & RI_KEY_E0) != 0;
		isE1 = (input.data.keyboard.Flags & RI_KEY_E1) != 0;
		keyUp = (input.data.keyboard.Flags & RI_KEY_BREAK) != 0;

		virtualKey = input.data.keyboard.VKey;

		if(virtualKey == VK_SHIFT)
		{
			//correct left / right shift
			virtualKey = MapVirtualKey(input.data.keyboard.MakeCode, MAPVK_VSC_TO_VK_EX);
		}

		keyCode = MapVirtualKeyToKeyCode(virtualKey);

		CKeyEvent keyEvent(keyCode);

		if(keyUp)
		{
			g_keyStates[(uint8)keyCode] = false;
			CEventQueue::DispatchKeyReleasedEvent(*AWindow::GetFocusedWindow(), keyEvent);
		}
		else
		{
			g_keyStates[(uint8)keyCode] = true;
			CEventQueue::DispatchKeyPressedEvent(*AWindow::GetFocusedWindow(), keyEvent);
		}*/
	}
	else if(input.header.dwType == RIM_TYPEMOUSE)
	{
		extern bool g_mouseButtonStates[1];

		//keyboard modifiers
		Events::KeyboardModifiers keyboardModifiers;

		keyboardModifiers.ctrl = GetKeyState(VK_CONTROL) & 0x8000; //TODO: update this from above

		//get cursor pos
		POINT cursorPos;
		::GetCursorPos(&cursorPos);
		static POINT lastCursorPos = cursorPos;

		//get the widget
		/*
		HWND hWnd = WindowFromPoint(cursorPos);
		CommCtrlWidgetBackend *backend = WindowsMessageQueueEventSource::GetAttachedBackend(hWnd);
		CommCtrlRenderTargetWidgetBackend *renderTargetWidgetBackend = dynamic_cast<CommCtrlRenderTargetWidgetBackend *>(backend);
		if (renderTargetWidgetBackend == nullptr)
			return;

		Widget &widget = backend->GetWidget();
		*/

		//convert cursor pos
		POINT clientCursorPos = cursorPos;
		ScreenToClient(hWnd, &clientCursorPos);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		PointD transformed = PointD(clientCursorPos.x, rcClient.bottom - clientCursorPos.y);
		bool cursorInWindow = PtInRect(&rcClient, clientCursorPos) != 0;

		//did cursor move?		
		bool cursorMoved = (lastCursorPos.x != cursorPos.x) || (lastCursorPos.y != cursorPos.y);
		if (cursorMoved && cursorInWindow)
			this->DispatchMouseMovedEvent(widget, transformed);

		//was mouse wheel used?
		if ((input.data.mouse.usButtonFlags & RI_MOUSE_WHEEL) && cursorInWindow)
			this->DispatchMouseWheelEvent(widget, (int16)input.data.mouse.usButtonData / WHEEL_DELTA);

		Events::MouseClickEvent eventl(MouseButton::Left, transformed, keyboardModifiers);
		if ((input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN))
		{
			//left mouse state changed to down
			this->DispatchMouseButtonPressed(widget, eventl);
		}
		else if ((input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP))
		{
			//left mouse state changed to up
			this->DispatchMouseButtonReleased(widget, eventl);
		}

		Events::MouseClickEvent eventr(MouseButton::Right, transformed, keyboardModifiers);
		if ((input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN))
		{
			//right mouse state changed to down
			this->DispatchMouseButtonPressed(widget, eventr);
		}
		else if ((input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP))
		{
			//right mouse state changed to up
			this->DispatchMouseButtonReleased(widget, eventr);
		}
		
		/*
		if(dynamic_cast<APopupWindow *>(pWnd))
		{
			uint32 packedCoord;
			LRESULT hitTestRes;

			packedCoord = MAKE32(cursorPos.x, cursorPos.y);

			hitTestRes = SendMessage(hWnd, WM_NCHITTEST, 0, packedCoord);
			if(hitTestRes != HTCLIENT)
			{
				WPARAM wParam;
				POINTS pts;

				wParam = 0; //TODO: set extended key modifiers

				pts.x = (SHORT)cursorPos.x;
				pts.y = (SHORT)cursorPos.y;

				if(mouseMoved)
					//PostMessage(hWnd, WM_MOUSEMOVE, wParam, packedCoord);
					PostMessage(hWnd, WM_NCMOUSEMOVE, hitTestRes, (LPARAM)&pts);
				//left mouse button
				if(input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
					PostMessage(hWnd, WM_NCLBUTTONDOWN, hitTestRes, (LPARAM)&pts);
				if(input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
					PostMessage(hWnd, WM_NCLBUTTONUP, hitTestRes, (LPARAM)&pts);
				//right mouse button
				if(input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
					PostMessage(hWnd, WM_NCRBUTTONDOWN, hitTestRes, (LPARAM)&pts);
				if(input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
					PostMessage(hWnd, WM_NCRBUTTONUP, hitTestRes, (LPARAM)&pts);

				goto endMouse;
			}
		}

		if(dynamic_cast<AWindowContainer *>(pWnd))
			pWnd = &((AWindowContainer *)pWnd)->Find(matchedX, matchedY);
		}*/

		//update state
		lastCursorPos = cursorPos;
	}
	else
	{
		g_ignoreMessage = true;
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
		case WM_INPUT:
		{
			this->DispatchInputEvent(hWnd, widget, (HRAWINPUT)lParam);
			l_messageResult = 0;
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
CommCtrlWidgetBackend *WindowsMessageQueueEventSource::GetAttachedBackend(HWND hWnd)
{
	if (!hWnd)
		return nullptr;

	//check class name
	wchar_t className[STDPLUSPLUS_WIN_WNDCLASS_LENGTH];
	GetClassNameW(hWnd, className, sizeof(className) / sizeof(className[0]));
	if(MemCmp(className, STDPLUSPLUS_WIN_WNDCLASS, STDPLUSPLUS_WIN_WNDCLASS_LENGTH) == 0)
		return (CommCtrlWidgetBackend *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	return nullptr;
}

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
		backend = WindowsMessageQueueEventSource::GetAttachedBackend(hWnd);
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