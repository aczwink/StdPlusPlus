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
#include "Win32Window.hpp"
//Local
#include <Std++/UI/Window.hpp>
#include "CommCtrlWindowBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Destructor
Win32Window::~Win32Window()
{
	DestroyWindow(this->hWnd);
}

//Public methods
String Win32Window::GetText() const
{
	const uint16 nCodeUnits = 2048;
	uint16 buffer[nCodeUnits]; //should be sufficient for most cases
	uint32 length = this->SendMessage(WM_GETTEXTLENGTH, 0, 0);
	if (length > nCodeUnits)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	uint32 nCopied = this->SendMessage(WM_GETTEXT, sizeof(buffer) / sizeof(buffer[0]), (LPARAM)buffer);
	buffer[nCopied] = 0;

	return String::CopyRawString(buffer);
}

Size<uint16> Win32Window::GetTextExtents() const
{
	HDC hDC = GetDC(this->GetHWND());
	HGDIOBJ oldFont = SelectObject(hDC, this->GetFont());

	String str = this->GetText();
	SIZE size;
	GetTextExtentPoint32W(hDC, (LPCWSTR)str.GetRawData(), str.GetLength(), &size);

	SelectObject(hDC, oldFont);
	ReleaseDC(this->hWnd, hDC);

	return Size<uint16>(size.cx, size.cy);
}

void Win32Window::SetParent(Win32Window *parent)
{
	NOT_IMPLEMENTED_ERROR; //TODO: reimplement
	/*
	parent->Realize();
	this->Realize();

	DWORD dwStyle = this->dwStyle;
	if (&this->widgetBackend.GetWidget() != this->widgetBackend.GetWidget().GetWindow())
	{
		//we change style only if this is not a window
		dwStyle |= WS_CHILD;
		dwStyle |= WS_VISIBLE;
	}

	::SetParent(this->hWnd, parent->hWnd);
	SetWindowLongPtrW(this->hWnd, GWL_STYLE, dwStyle);
	SetWindowPos(this->hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	*/
}

//Private methods
void Win32Window::CreateHWND() const
{
	//find a suitable parent
	const Widget *current = this->backend.GetWidget().GetParent();
	while (current && current->_GetBackend() == nullptr)
		current = current->GetParent();

	const Win32Window* parent = nullptr;
	HWND hParent = nullptr;
	if (current)
	{
		const CommCtrlWidgetBackend *parentBackend = dynamic_cast<const CommCtrlWidgetBackend *>(current->_GetBackend());
		parent = parentBackend->GetFirstWindow();
		hParent = parent->GetHWND();
	}

	//set style
	DWORD dwStyle = this->dwStyle;
	if (parent)
	{
		bool isWindow = &this->backend.GetWidget() == this->backend.GetWidget().GetWindow();
		if (isWindow)
			dwStyle |= WS_POPUP; //window with other
		else
		{
			dwStyle |= WS_CHILD; //its a child
								 //dwStyle |= WS_VISIBLE; //children default to be visible
		}
	}
	else
	{
		dwStyle |= WS_OVERLAPPED;
	}

	//position
	int x = 0;
	int width = 0;
	if ((dwStyle & (WS_POPUP | WS_CHILD)) == WS_OVERLAPPED) //is it a top-level window?
	{
		x = CW_USEDEFAULT;
		width = CW_USEDEFAULT;
	}

	//create HWND
	HINSTANCE hInstance = GetModuleHandle(NULL);
	this->hWnd = CreateWindowExW(dwExStyle, lpClassName, nullptr, dwStyle, x, 0, width, 0, hParent, NULL, hInstance, (LPVOID)&backend);

	ASSERT(this->hWnd, u8"Realization failed");

	//set stuff on HWND
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)&backend);
	this->SendMessage(WM_SETFONT, (WPARAM)this->GetFont(), TRUE);
}