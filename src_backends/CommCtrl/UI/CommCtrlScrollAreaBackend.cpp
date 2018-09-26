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
#include "CommCtrlScrollAreaBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
WidgetContainerBackend* CommCtrlScrollAreaBackend::CreateContentAreaBackend(CompositeWidget & widget)
{
	return nullptr;
}

RectD CommCtrlScrollAreaBackend::GetContentAreaBounds() const
{
	SizeD contentSize = this->scrollArea->GetContentContainer()->GetSizeHint();
	SizeD scrollAreaSize = this->GetWidget().GetSize();

	SizeD allocationSize = contentSize.Max(scrollAreaSize);
	if (contentSize.height > scrollAreaSize.height)
		allocationSize.width -= GetSystemMetrics(SM_CXVSCROLL); //scroll width

	return RectD(PointD(), allocationSize);
}

SizeD CommCtrlScrollAreaBackend::GetSizeHint() const
{
	//TODO: actually sizes of both scroll bars...
	return SizeD(); //no requirements
}

void CommCtrlScrollAreaBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(this->GetHWND(), &ps);

		HBRUSH hBrush = GetSysColorBrush(COLOR_WINDOW);
		FillRect(ps.hdc, &ps.rcPaint, hBrush);

		EndPaint(this->GetHWND(), &ps);

		event.consumed = true;
		event.result = 0;
	}
	break;
	case WM_VSCROLL:
	{
		switch (LOWORD(event.wParam))
		{
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		UINT nLines;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nLines, 0);

		int16 dy = GET_WHEEL_DELTA_WPARAM(event.wParam) / WHEEL_DELTA;
		dy *= nLines * 10;
		this->Scroll(0, -dy);

		event.consumed = true;
		event.result = 0;
	}
	break;
	}
}

void CommCtrlScrollAreaBackend::SetBounds(const RectD &bounds)
{
	CommCtrlWidgetBackend::SetBounds(bounds);

	RectD contentBounds = this->GetContentAreaBounds();
	
	ShowScrollBar(this->GetHWND(), SB_HORZ, contentBounds.width() > bounds.width());

	//vertical bar
	if (contentBounds.height() > bounds.height())
	{
		SCROLLINFO scrollInfo;
		scrollInfo.cbSize = sizeof(scrollInfo);
		scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = int(contentBounds.height());
		scrollInfo.nPage = int(bounds.height());

		SetScrollInfo(this->GetHWND(), SB_VERT, &scrollInfo, TRUE);
		ShowScrollBar(this->GetHWND(), SB_VERT, TRUE);
	}
	else
		ShowScrollBar(this->GetHWND(), SB_VERT, FALSE);
}

//Private methods
void CommCtrlScrollAreaBackend::Scroll(int32 dx, int32 dy)
{
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(scrollInfo);
	scrollInfo.fMask = SIF_POS;
	GetScrollInfo(this->GetHWND(), SB_HORZ, &scrollInfo);
	int32 oldX = scrollInfo.nPos;

	GetScrollInfo(this->GetHWND(), SB_VERT, &scrollInfo);
	int32 oldY = scrollInfo.nPos;

	if (dy)
	{
		scrollInfo.nPos += dy;
		SetScrollInfo(this->GetHWND(), SB_VERT, &scrollInfo, TRUE);
	}

	GetScrollInfo(this->GetHWND(), SB_HORZ, &scrollInfo);
	int32 newX = scrollInfo.nPos;

	GetScrollInfo(this->GetHWND(), SB_VERT, &scrollInfo);
	int32 newY = scrollInfo.nPos;
	dy = -(newY - oldY);

	if(dx || dy)
		ScrollWindowEx(this->GetHWND(), newX - oldX, dy, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE | SW_SCROLLCHILDREN);
}




















//Not implemented:
void _stdxx_::CommCtrlScrollAreaBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetEnabled(bool enable)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}