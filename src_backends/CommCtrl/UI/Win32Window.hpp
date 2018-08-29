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
#pragma once
//Local
#include <Std++/_Backends/UI/WidgetBackend.hpp>
#include "../Imports.h"

namespace _stdxx_
{
	class Win32Window
	{
	public:
		//Constructor
		inline Win32Window(const WidgetBackend &widgetBackend, LPCWSTR lpClassName, DWORD dwStyle = 0, DWORD dwExStyle = 0)
			: widgetBackend(widgetBackend), lpClassName(lpClassName), dwStyle(dwStyle), dwExStyle(dwExStyle), hWnd(nullptr)
		{
		}

		//Destructor
		~Win32Window();

		//Inline
		inline HWND GetHWND()
		{
			this->Realize();
			return this->hWnd;
		}

		inline void SetText(const StdXX::String &text)
		{
			this->SendMessage(WM_SETTEXT, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
		}

	private:
		//Members
		const WidgetBackend &widgetBackend;
		HWND hWnd;
		LPCWSTR lpClassName;
		DWORD dwStyle;
		DWORD dwExStyle;

		//Methods
		void Realize();

		//Inline
		inline HFONT GetFont() const
		{
			return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		}

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const
		{
			return SendMessageW(this->hWnd, Msg, wParam, lParam);
		}
	};
}