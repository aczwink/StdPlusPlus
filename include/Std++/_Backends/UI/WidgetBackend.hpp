/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/Path.hpp>
#include <Std++/Math/Geometry/Rect.hpp>
#include <Std++/Math/Size.hpp>
#include <Std++/Function.hpp>

//Forward declarations
namespace StdXX
{
	class UIBackend;

	namespace UI
	{
		class MenuBar;
		class Widget;
	}
}

namespace _stdxx_
{
	//Forward declarations
	class MenuBarBackend;

	class WidgetBackend
	{
	public:
		//Constructor
		inline WidgetBackend(StdXX::UIBackend& uiBackend)
			: uiBackend(uiBackend)
		{
		}

		//Destructor
		virtual ~WidgetBackend()
		{
		}

		//Abstract
		virtual StdXX::Math::SizeD GetSizeHint() const = 0;
		virtual const StdXX::UI::Widget &GetWidget() const = 0;
		virtual void Repaint() = 0;
		virtual void SetBounds(const StdXX::Math::RectD &bounds) = 0;
		virtual void SetEnabled(bool enable) = 0;
		virtual void SetHint(const StdXX::String &text) = 0;
		virtual void Show(bool visible) = 0;

		//Inline
		inline StdXX::UI::Widget& GetEditableWidget()
		{
			return const_cast<StdXX::UI::Widget &>(this->GetWidget());
		}

		inline StdXX::UIBackend& GetUIBackend() const
		{
			return this->uiBackend;
		}

	private:
		//Members
		StdXX::UIBackend& uiBackend;
	};
}