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

//Destructor
CommCtrlGroupBoxBackend::~CommCtrlGroupBoxBackend()
{
	if (this->hTheme)
		CloseThemeData(this->hTheme);
}

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
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(this->GetHWND(), &ps);
		this->Paint(hDC);
		EndPaint(this->GetHWND(), &ps);

		event.consumed = true;
		event.result = 0;
	}
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
	case WM_PRINTCLIENT:
	{
		this->Paint((HDC)event.wParam);

		event.consumed = true;
		event.result = 0;
	}
	break;
	case WM_THEMECHANGED:
	{
		if (this->hTheme)
			CloseThemeData(this->hTheme);
		this->hTheme = OpenThemeData(this->GetHWND(), WC_BUTTONW);
		this->groupBox->Repaint();
	}
	break;
	default:
		CommCtrlContainerBackend::OnMessage(event);
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
void CommCtrlGroupBoxBackend::Paint(HDC hDC)
{
	HWND hWnd = this->GetHWND();
	if (this->hTheme == nullptr)
		this->hTheme = OpenThemeData(hWnd, WC_BUTTONW);

	GROUPBOXSTATES state = GBS_DISABLED;
	if (IsWindowEnabled(hWnd))
		state = GBS_NORMAL;

	HFONT hFont = (HFONT)this->SendMessage(WM_GETFONT, 0, 0);
	SelectObject(hDC, hFont);

	//compute rects
	RECT clientRect;
	::GetClientRect(hWnd, &clientRect);

#if WINVER == _WIN32_WINNT_WIN7
	/*
	on windows 7 we need to manually paint the background, as WS_CLIPCHILDREN in the parent
	will prevent that the parent actually paints the background.
	*/
	HBRUSH hBrush = GetSysColorBrush(COLOR_WINDOW);
	FillRect(hDC, &clientRect, hBrush);
#endif

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
		DrawThemeParentBackground(hWnd, hDC, nullptr); //sends WM_PRINTCLIENT to parent
	DrawThemeBackground(hTheme, hDC, BP_GROUPBOX, state, &backgroundRect, nullptr);

	SelectClipRgn(hDC, nullptr);

	const String& text = this->groupBox->GetTitle();
	textRect.left += 2;
	textRect.right -= 2;
	DrawThemeText(hTheme, hDC, BP_GROUPBOX, state, (LPCWSTR)text.ToUTF16().GetRawZeroTerminatedData(), text.GetLength(), 0, 0, &textRect);
}












//TODO:
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