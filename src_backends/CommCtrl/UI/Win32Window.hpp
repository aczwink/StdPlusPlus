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
#include "CommCtrlWidgetBackend.hpp"
#include "../Imports.h"

namespace _stdxx_
{
	class Win32Window
	{
	public:
		//Constructor
		inline Win32Window(const CommCtrlWidgetBackend &widgetBackend, LPCWSTR lpClassName, DWORD dwStyle = 0, DWORD dwExStyle = 0)
			: widgetBackend(widgetBackend), lpClassName(lpClassName), dwStyle(dwStyle), dwExStyle(dwExStyle), hWnd(nullptr)
		{
		}

		//Destructor
		~Win32Window();

		//Methods
		StdXX::String GetText() const;
		StdXX::Math::Size<uint16> GetTextExtents() const;
		void SetParent(Win32Window *parent);

		//Inline
		inline HWND GetHWND()
		{
			this->Realize();
			return this->hWnd;
		}

		inline HWND Get_HWND_ReadOnly() const
		{
			ASSERT(this->hWnd, u8"Window handle not realized");
			return this->hWnd;
		}

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const
		{
			return SendMessageW(this->Get_HWND_ReadOnly(), Msg, wParam, lParam);
		}

		inline void SetRect(const StdXX::Math::RectD &bounds)
		{
			SetWindowPos(this->GetHWND(), HWND_TOP, bounds.x(), bounds.y(), bounds.width(), bounds.height(), SWP_NOZORDER);
		}

		inline void SetText(const StdXX::String &text)
		{
			this->SendMessage(WM_SETTEXT, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
		}

	private:
		//Members
		const CommCtrlWidgetBackend &widgetBackend;
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

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
		{
			return SendMessageW(this->GetHWND(), Msg, wParam, lParam);
		}
	};
}