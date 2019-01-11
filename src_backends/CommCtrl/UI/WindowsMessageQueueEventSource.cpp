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
#include <Std++/UI/Views/SelectBox.hpp>
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

		TranslateMessage(&msg); //TODO: this is needed when not managing the whole keyboard with RegisterRawInputDevices
		DispatchMessage(&msg);
	}
}

uint64 WindowsMessageQueueEventSource::GetMaxTimeout() const
{
	return Unsigned<uint64>::Max();
}

void WindowsMessageQueueEventSource::VisitWaitObjects(const Function<void(_stdxx_::WaitObjHandle, bool)>& visitFunc)
{
}

//Private methods
	//if(input.header.dwType == RIM_TYPEKEYBOARD)
	//{
		/*
		source:
		http://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/
		*/

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
	//}

/*bool WindowsMessageQueueEventSource::DispatchMessageEvent(CommCtrlWidgetBackend &backend, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_ignoreMessage = false;
	
	Widget &widget = backend.GetWidget();
	Window *window = dynamic_cast<Window *>(&widget);
	
	switch(message)
	{
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
}*/

void WindowsMessageQueueEventSource::DispatchNotificationEvent(Widget &refWidget, const NMHDR &refNmHdr)
{
	switch (refNmHdr.code)
	{
	case TVN_SELCHANGEDW:
	{
		TreeView &refTreeView = (TreeView &)refWidget;

		//this->DispatchSelectionChangedEvent(refTreeView);
	}
	break;
	}
}

//Class functions
CommCtrlWidgetBackend *WindowsMessageQueueEventSource::GetAttachedBackend(HWND hWnd)
{
	if (!hWnd)
		return nullptr;
	
	return (CommCtrlWidgetBackend *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
}

CommCtrlWidgetBackend *WindowsMessageQueueEventSource::GetAttachedBackendIfStdXXWnd(HWND hWnd)
{
	if (!hWnd)
		return nullptr;

	//check class name
	wchar_t className[STDPLUSPLUS_WIN_WNDCLASS_LENGTH];
	GetClassNameW(hWnd, className, sizeof(className) / sizeof(className[0]));
	if (MemCmp(className, STDPLUSPLUS_WIN_WNDCLASS, STDPLUSPLUS_WIN_WNDCLASS_LENGTH) == 0)
		return WindowsMessageQueueEventSource::GetAttachedBackend(hWnd);
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

	if (backend)
	{
		WinMessageEvent event;
		event.hWnd = hWnd;
		event.message = message;
		event.wParam = wParam;
		event.lParam = lParam;

		backend->OnMessage(event);
		if (event.consumed)
			return event.result;
	}
	return DefWindowProcW(hWnd, message, wParam, lParam);
}

/*
void WinEventQueueBackend::PostQuitEvent()
{
	PostQuitMessage(EXIT_SUCCESS);
}
*/