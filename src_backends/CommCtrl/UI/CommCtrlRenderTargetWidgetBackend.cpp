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
#include "CommCtrlRenderTargetWidgetBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Constructor
CommCtrlRenderTargetWidgetBackend::CommCtrlRenderTargetWidgetBackend(StdXX::UIBackend& uiBackend, StdXX::UI::RenderTargetWidget *renderTargetWidget)
		: CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, STDPLUSPLUS_WIN_WNDCLASS), renderTargetWidget(renderTargetWidget)
{
	//register keyboard input
	RAWINPUTDEVICE inputDev;
	inputDev.dwFlags = 0;
	//inputDev.dwFlags = RIDEV_NOLEGACY; //TODO: this does only work when we manage the whole window, since this will somehow affect ALL windows this widget participates in (even parents-.-)
	//commctrl widgets won't receive any key events then
	inputDev.hwndTarget = this->GetHWND();
	inputDev.usUsagePage = 1; //'generic desktop' from USB HID usage table
	inputDev.usUsage = 6; //keyboard

	//RegisterRawInputDevices(&inputDev, 1, sizeof(RAWINPUTDEVICE));

	//register mouse input
	inputDev.dwFlags = 0; //TODO: unfortunately we need legacy messages for windows. If ever windows are custom drawn this can be changed
	inputDev.usUsage = 2;

	RegisterRawInputDevices(&inputDev, 1, sizeof(RAWINPUTDEVICE));

	this->mouseButtonStates[0] = false;
	this->mouseButtonStates[1] = false;
}

//Public methods
SizeD CommCtrlRenderTargetWidgetBackend::GetSizeHint() const
{
	return SizeD();
}

Widget & CommCtrlRenderTargetWidgetBackend::GetWidget()
{
	return *this->renderTargetWidget;
}

const StdXX::UI::Widget & _stdxx_::CommCtrlRenderTargetWidgetBackend::GetWidget() const
{
	return *this->renderTargetWidget;
}

void CommCtrlRenderTargetWidgetBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_PAINT:
	{
		//validate the update region (it is important that this is done before calling the event handler because user might want to redraw in paint handler!!!)
		RECT rcUpdate;
		GetUpdateRect(this->GetHWND(), &rcUpdate, FALSE);
		ValidateRect(this->GetHWND(), &rcUpdate);

		Event e(EventType::WidgetShouldBePainted);
		this->renderTargetWidget->Event(e);
		if (e.WasAccepted())
			event.consumed = true;
		else
		{
			event.consumed = false;
			//we validated the rect errornously -> invalidate again
			InvalidateRect(this->GetHWND(), &rcUpdate, FALSE);
		}

		event.result = 0;
	}
	break;
	case WM_INPUT:
	{
		RAWINPUT input;
		UINT size = sizeof(input);
		GetRawInputData((HRAWINPUT)event.lParam, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER));

		event.consumed = true;
		event.result = 0;

		if (input.header.dwType == RIM_TYPEKEYBOARD)
			;//TODO: reimplement me
		else if (input.header.dwType == RIM_TYPEMOUSE)
			this->DispatchMouseEvents(event.hWnd, input);
		else
			event.consumed = false;
	}
	break;
	}
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
void CommCtrlRenderTargetWidgetBackend::DispatchMouseEvents(HWND hWnd, RAWINPUT& input)
{
	//keyboard modifiers
	KeyboardModifiers keyboardModifiers;

	keyboardModifiers.ctrl = GetKeyState(VK_CONTROL) & 0x8000;
	
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
	::GetClientRect(hWnd, &rcClient);
	PointD transformed = PointD(clientCursorPos.x, rcClient.bottom - clientCursorPos.y);
	bool cursorInWindow = PtInRect(&rcClient, clientCursorPos) != 0;

	//did cursor move?		
	bool cursorMoved = (lastCursorPos.x != cursorPos.x) || (lastCursorPos.y != cursorPos.y);
	if (cursorMoved && cursorInWindow)
	{
		MouseEvent mouseEvent(EventType::MouseMoved, transformed, keyboardModifiers);
		this->renderTargetWidget->Event(mouseEvent);
	}

	//was mouse wheel used?
	if ((input.data.mouse.usButtonFlags & RI_MOUSE_WHEEL) && cursorInWindow)
	{
		int16 dy = (int16)input.data.mouse.usButtonData / WHEEL_DELTA;
		MouseWheelEvent mouseWheelEvent(dy, keyboardModifiers);
		this->renderTargetWidget->Event(mouseWheelEvent);
		SetFocus(hWnd);
	}

	if ((input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) && cursorInWindow)
	{
		//left mouse state changed to down
		SetFocus(hWnd);

		MouseClickEvent event(MouseButton::Left, true, transformed, keyboardModifiers);
		this->renderTargetWidget->Event(event);

		this->mouseButtonStates[0] = true;
	}
	else if ((input.data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) && this->mouseButtonStates[0])
	{
		//left mouse state changed to up
		MouseClickEvent event(MouseButton::Left, false, transformed, keyboardModifiers);
		this->renderTargetWidget->Event(event);

		this->mouseButtonStates[0] = false;
	}

	if ((input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) && cursorInWindow)
	{
		//right mouse state changed to down
		SetFocus(hWnd);

		MouseClickEvent event(MouseButton::Right, true, transformed, keyboardModifiers);
		this->renderTargetWidget->Event(event);

		this->mouseButtonStates[1] = true;
	}
	else if ((input.data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) && this->mouseButtonStates[1])
	{
		//right mouse state changed to up
		MouseClickEvent event(MouseButton::Right, false, transformed, keyboardModifiers);
		this->renderTargetWidget->Event(event);

		this->mouseButtonStates[1] = false;
	}

	//update state
	lastCursorPos = cursorPos;

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
}