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
#include "CommCtrlGroupBoxBackend.hpp"
//Global
#include <Uxtheme.h>
#include <vsstyle.h>
//Local
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "CommCtrlContainerBackend.hpp"
#include "WindowsMessageQueueEventSource.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
WidgetContainerBackend * CommCtrlGroupBoxBackend::CreateContentAreaBackend(CompositeWidget & widget)
{
	return nullptr;
}

RectD CommCtrlGroupBoxBackend::GetContentAreaBounds() const
{
	Size<uint16> titleSize = this->GetTextExtents();
	titleSize.width += 10 + 4 - 2;

	RectD result;
	result.size = this->groupBox->GetSize();
	result.height() -= titleSize.height / 1.5;
	return result;
}

SizeD CommCtrlGroupBoxBackend::GetSizeHint() const
{
	SizeD titleSize = this->GetTextExtents().Cast<float64>();
	titleSize.width += 10 + 4 - 2;

	return titleSize;
}

Widget &CommCtrlGroupBoxBackend::GetWidget()
{
	return *this->groupBox;
}

const Widget &CommCtrlGroupBoxBackend::GetWidget() const
{
	return *this->groupBox;
}

void CommCtrlGroupBoxBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_PAINT:
		this->Paint();
		event.consumed = true;
		event.result = 0;
		break;
	case WM_SETFONT:
		event.consumed = true;
		event.result = 0;
		this->hFont = (HFONT)event.wParam;
		break;
	case WM_GETFONT:
		event.consumed = true;
		event.result = (LRESULT)this->hFont;
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
	}
}

void CommCtrlGroupBoxBackend::SetBounds(const RectD &bounds)
{
	this->SetRect(this->ToWinAPIBounds(bounds));
}

void CommCtrlGroupBoxBackend::SetTitle(const String &title)
{
	this->SetText(title);
}

//Private methods
void CommCtrlGroupBoxBackend::Paint()
{
	PAINTSTRUCT ps;
	HWND hWnd = this->GetHWND();
	HTHEME hTheme = GetWindowTheme(hWnd);
	if (hTheme == nullptr)
		hTheme = OpenThemeData(hWnd, WC_BUTTONW);

	HDC hDC = BeginPaint(hWnd, &ps);

	GROUPBOXSTATES state = GBS_DISABLED;
	if (IsWindowEnabled(hWnd))
		state = GBS_NORMAL;

	HFONT hFont = (HFONT)this->SendMessage(WM_GETFONT, 0, 0);
	SelectObject(hDC, hFont);

	//compute rects
	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);

	RECT textRect;
	auto textExtent = this->GetTextExtents();
	textRect.left = clientRect.left + 10;
	textRect.right = textRect.left + textExtent.width + 4;
	textRect.top = clientRect.top;
	textRect.bottom = textRect.top + textExtent.height;
	ExcludeClipRect(hDC, textRect.left, textRect.top, textRect.right, textRect.bottom);

	RECT backgroundRect = clientRect;
	backgroundRect.top += textExtent.height / 2;

	RECT contentRect;
	GetThemeBackgroundContentRect(hTheme, hDC, BP_GROUPBOX, state, &backgroundRect, &contentRect);
	ExcludeClipRect(hDC, contentRect.left, contentRect.top, contentRect.right, contentRect.bottom);

	//draw
	if (IsThemeBackgroundPartiallyTransparent(hTheme, BP_GROUPBOX, state))
		DrawThemeParentBackground(hWnd, hDC, nullptr);
	DrawThemeBackground(hTheme, hDC, BP_GROUPBOX, state, &backgroundRect, nullptr);

	SelectClipRgn(hDC, nullptr);

	const String& text = this->groupBox->GetTitle();
	textRect.left += 2;
	textRect.right -= 2;
	DrawThemeText(hTheme, hDC, BP_GROUPBOX, state, (LPCWSTR)text.ToUTF16().GetRawZeroTerminatedData(), text.GetLength(), 0, 0, &textRect);

	EndPaint(hWnd, &ps);

	CloseThemeData(hTheme);
}












//TODO:

void _stdxx_::CommCtrlGroupBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}