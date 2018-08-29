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
using namespace StdXX::UI;

//Destructor
Win32Window::~Win32Window()
{
	DestroyWindow(this->hWnd);
}

//Private methods
void Win32Window::Realize()
{
	//already realized?
	if (this->hWnd != nullptr)
		return;

	//get parent window
	bool selfIsWindow = false;
	Window *parentWindow = (Window *)this->widgetBackend.GetWidget().GetWindow();
	if (&this->widgetBackend.GetWidget() == parentWindow)
	{
		selfIsWindow = true;
		if (parentWindow->GetParent())
			parentWindow = parentWindow->GetParent()->GetWindow();
		else
			parentWindow = nullptr;
	}

	//get parent HWND	
	HWND hParent = nullptr;
	if(parentWindow)
	{
		CommCtrlWindowBackend *windowBackend = dynamic_cast<CommCtrlWindowBackend *>(parentWindow->_GetBackend());
		hParent = windowBackend->GetHWND();
	}

	//set style
	DWORD dwStyle = this->dwStyle;
	if (!selfIsWindow)
	{
		if (hParent == nullptr)
			dwStyle |= WS_POPUP; //the widget does not have a parent yet :S
		else
		{
			dwStyle |= WS_CHILD; //its a child
			dwStyle |= WS_VISIBLE; //children default to be visible
		}
	}

	//create HWND
	HINSTANCE hInstance = GetModuleHandle(NULL);
	this->hWnd = CreateWindowExW(this->dwExStyle, this->lpClassName, nullptr, this->dwStyle, 0, 0, 0, 0, hParent, NULL, hInstance, (LPVOID)&this->widgetBackend);

	ASSERT(this->hWnd, u8"Realization failed");

	//set stuff on HWND
	SetWindowLongPtr(this->hWnd, GWLP_USERDATA, (LONG_PTR)&this->widgetBackend);
	this->SendMessage(WM_SETFONT, (WPARAM)this->GetFont(), TRUE);
}