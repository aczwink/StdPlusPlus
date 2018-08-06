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
#include "Std++/Containers/Strings/String.hpp"
#include "Std++/Filesystem/Path.hpp"
#include "Std++/Math/Geometry/Rect.hpp"
#include "Std++/Math/Size.hpp"
#include "Std++/UI/Controllers/SelectionController.hpp"
#include "Std++/Function.hpp"

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
		inline WidgetBackend(StdXX::UIBackend *uiBackend)
			: uiBackend(uiBackend)
		{
		}

		//Destructor
		virtual ~WidgetBackend()
		{
		}

		//Abstract
		virtual StdXX::Math::SizeD GetSize() const = 0;
		virtual StdXX::Math::SizeD GetSizeHint() const = 0;
		virtual void Paint() = 0;
		virtual void Repaint() = 0;
		virtual void Select(StdXX::UI::ControllerIndex &controllerIndex) const = 0;
		virtual void SetBounds(const StdXX::Math::RectD &area) = 0;
		virtual void SetEditable(bool enable) const = 0;
		virtual void SetEnabled(bool enable) const = 0;
		virtual void SetHint(const StdXX::String &text) const = 0;
		virtual void Show(bool visible) = 0;
		virtual void ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const = 0;
		virtual void UpdateSelection(StdXX::UI::SelectionController &selectionController) const = 0;

		//Abstract, for Widget
		virtual void IgnoreEvent() = 0;

		//Abstract, for Slider
		virtual uint32 GetPosition() const = 0;

		//Abstract, for SpinBox
		virtual void GetRange(int32 &min, int32 &max) = 0;

		//Abstract, for View
		virtual void ResetView() const = 0;

		//Abstract, for Window
		virtual void SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend) = 0;

		//Inline
		inline StdXX::UIBackend *GetUIBackend() const
		{
			return this->uiBackend;
		}

	private:
		//Members
		StdXX::UIBackend * uiBackend;
	};
}