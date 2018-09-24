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

namespace _stdxx_
{
	//Forward declarations
	class Win32Window;

	class CommCtrlWidgetBackend : virtual public WidgetBackend
	{
	public:
		//Constructor
		inline CommCtrlWidgetBackend(StdXX::UIBackend *uiBackend) : WidgetBackend(uiBackend)
		{
		}

		//Overrideable
		virtual const Win32Window* GetNextWindow(const Win32Window* window) const;
		virtual void PrePaint();
		virtual void SetEnabled(bool enable) override;
		virtual void Show(bool visible) override;

		//Methods
		void IgnoreEvent() override;
		void Repaint() override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;

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