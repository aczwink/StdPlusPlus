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
#include "../Containers/Strings/String.hpp"
#include "../Filesystem/Path.hpp"
#include "../Math/Geometry/Rect.hpp"
#include "../Math/Size.hpp"
#include "../UI/Controllers/SelectionController.hpp"
#include "../Function.hpp"
#include "WindowBackendType.hpp"

//Forward declarations
namespace StdPlusPlus
{
	class UIBackend;

	namespace UI
	{
		class Widget;
	}
}

namespace _stdpp
{
	class WindowBackend
	{
	public:
		virtual ~WindowBackend()
		{
		}

		//Abstract
		virtual void ClearView() const = 0;
		virtual WindowBackend *CreateChildBackend(_stdpp::WindowBackendType type, StdPlusPlus::UI::Widget *widget) const = 0;
		virtual StdPlusPlus::Size GetSize() const = 0;
		virtual StdPlusPlus::Size GetSizeHint() const = 0;
		virtual StdPlusPlus::UIBackend *GetUIBackend() = 0;
		virtual void Paint() = 0;
		virtual void Repaint() = 0;
		virtual void Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const = 0;
		virtual StdPlusPlus::Path SelectExistingDirectory(const StdPlusPlus::String &title, const StdPlusPlus::Function<bool(StdPlusPlus::Path &)> callback) const = 0;
		virtual void SetBounds(const StdPlusPlus::Rect &area) = 0;
		virtual void SetEnabled(bool enable) const = 0;
		virtual void SetHint(const StdPlusPlus::String &text) const = 0;
		virtual void SetText(const StdPlusPlus::String &text) = 0;
		virtual void Show(bool visible) = 0;
		virtual void ShowInformationBox(const StdPlusPlus::String &title, const StdPlusPlus::String &message) const = 0;
		virtual void UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const = 0;
	};
}