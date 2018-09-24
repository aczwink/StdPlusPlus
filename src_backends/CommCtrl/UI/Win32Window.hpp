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
		inline Win32Window(CommCtrlWidgetBackend& backend, LPCWSTR lpClassName, DWORD dwStyle = 0, DWORD dwExStyle = 0)
			: hWnd(nullptr), backend(backend), lpClassName(lpClassName), dwStyle(dwStyle), dwExStyle(dwExStyle)
		{
		}

		//Destructor
		~Win32Window();

		//Methods
		StdXX::Math::RectD GetClientRect() const;
		StdXX::String GetText() const;
		StdXX::Math::Size<uint16> GetTextExtents() const;
		void SetParent(Win32Window *parent);

		//Inline
		inline HWND GetHWND()
		{
			if (this->hWnd == nullptr)
				this->CreateHWND();
			return this->hWnd;
		}

		inline const HWND GetHWND() const
		{
			if (this->hWnd == nullptr)
				this->CreateHWND();
			return this->hWnd;
		}

		inline void SetEnabled(bool enable) 
		{
			EnableWindow(this->GetHWND(), enable);
		}

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
		{
			return SendMessageW(this->GetHWND(), Msg, wParam, lParam);
		}

		inline LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const
		{
			return SendMessageW(this->GetHWND(), Msg, wParam, lParam);
		}

		inline void SetRect(const StdXX::Math::RectD &bounds)
		{
			SetWindowPos(this->GetHWND(), HWND_TOP, bounds.x(), bounds.y(), bounds.width(), bounds.height(), SWP_NOZORDER);
		}

		inline void SetText(const StdXX::String &text)
		{
			this->SendMessage(WM_SETTEXT, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
		}

		inline void Show(bool visible)
		{
			ShowWindow(this->GetHWND(), visible ? SW_SHOW : SW_HIDE);
		}

	private:
		//Members
		mutable HWND hWnd;
		CommCtrlWidgetBackend& backend;
		LPCWSTR lpClassName;
		DWORD dwStyle;
		DWORD dwExStyle;

		//Methods
		void CreateHWND() const;

		//Inline
		inline HFONT GetFont() const
		{
			return (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		}
	};
}