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
#include "CommCtrlContainerBackend.hpp"
//Local
#include "WindowsMessageQueueEventSource.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
void CommCtrlContainerBackend::AddChild(StdXX::UI::Widget *widget)
{
	if (this->container->GetParent())
	{
		CommCtrlContainerBackend *parentBackend = dynamic_cast<CommCtrlContainerBackend *>(this->container->GetParent()->_GetBackend());
		if(parentBackend)
			parentBackend->AddChild(widget);
	}
}

SizeD CommCtrlContainerBackend::GetSizeHint() const
{
	//no requirements here
	return StdXX::Math::SizeD();
}

Widget &CommCtrlContainerBackend::GetWidget()
{
	return *this->container;
}

const Widget &CommCtrlContainerBackend::GetWidget() const
{
	return *this->container;
}

void CommCtrlContainerBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(event.hWnd, &ps);

		HBRUSH hBrush = GetSysColorBrush(COLOR_WINDOW);
		FillRect(ps.hdc, &ps.rcPaint, hBrush);

		EndPaint(event.hWnd, &ps);

		event.consumed = true;
		event.result = 0;
	}
	break;
	case WM_NOTIFY:
	{
		const NMHDR* nmHeader = (NMHDR *)event.lParam;
		CommCtrlWidgetBackend* backend = WindowsMessageQueueEventSource::GetAttachedBackend(nmHeader->hwndFrom);
		backend->OnMessage(event); //forward message

		if(!event.consumed)
			CommCtrlWidgetBackend::OnMessage(event);
	}
	break;
	case WM_COMMAND:
	{
		if (event.lParam) //control-event
		{
			CommCtrlWidgetBackend* backend = WindowsMessageQueueEventSource::GetAttachedBackend((HWND)event.lParam);
			if (backend)
				backend->OnMessage(event);
		}
	}
	break;
	case WM_HSCROLL: //for track bar
	{
		CommCtrlWidgetBackend* backend = WindowsMessageQueueEventSource::GetAttachedBackend((HWND)event.lParam);
		if (backend)
			backend->OnMessage(event);
	}
	break;
	default:
		CommCtrlWidgetBackend::OnMessage(event);
	}
}

void CommCtrlContainerBackend::IgnoreEvent()
{
	//do nothing
}

void _stdxx_::CommCtrlContainerBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}