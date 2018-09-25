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
CommCtrlRenderTargetWidgetBackend::CommCtrlRenderTargetWidgetBackend(StdXX::UIBackend *uiBackend, StdXX::UI::RenderTargetWidget *renderTargetWidget)
		: CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, STDPLUSPLUS_WIN_WNDCLASS), renderTargetWidget(renderTargetWidget)
{
	//register keyboard input
	RAWINPUTDEVICE inputDev;
	inputDev.dwFlags = RIDEV_NOLEGACY;
	inputDev.hwndTarget = this->GetHWND();
	inputDev.usUsagePage = 1; //'generic desktop' from USB HID usage table
	inputDev.usUsage = 6; //keyboard

	RegisterRawInputDevices(&inputDev, 1, sizeof(RAWINPUTDEVICE));

	//register mouse input
	inputDev.dwFlags = 0; //TODO: unfortunately we need legacy messages for windows. If ever windows are custom drawn this can be changed
	inputDev.usUsage = 2;

	RegisterRawInputDevices(&inputDev, 1, sizeof(RAWINPUTDEVICE));
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

		event.consumed = true;
		event.result = 0;
	}
	break;
	}
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
