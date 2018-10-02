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
	//Forward declarations
	class CommCtrlWidgetBackend;
	class Win32Window;

	struct WinMessageEvent
	{
		HWND hWnd;
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
		bool consumed = false;
		LRESULT result;
	};

	class CommCtrlWidgetBackend : virtual public WidgetBackend
	{
	public:
		//Members
		WNDPROC origWndProc;

		//Constructor
		inline CommCtrlWidgetBackend(StdXX::UIBackend& uiBackend) : WidgetBackend(uiBackend),
			origWndProc(nullptr)
		{
		}

		//Overrideable
		virtual const Win32Window* GetNextWindow(const Win32Window* window) const;
		virtual void OnMessage(WinMessageEvent& event);
		virtual void SetEnabled(bool enable) override;
		virtual void Show(bool visible) override;

		//Methods
		void IgnoreEvent() override;
		void Repaint() override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;
		void SetHint(const StdXX::String & text) override;

		//Inline
		inline const Win32Window* GetFirstWindow() const
		{
			return this->GetNextWindow(nullptr);
		}

	protected:
		//Methods
		/*
		Translates bounds into WinAPI Coordinate system i.e. (0,0) is the top-left corner.
		*/
		StdXX::Math::RectD ToWinAPIBounds(const StdXX::Math::RectD &bounds) const;
	};
}