/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../Containers/Strings/String.hpp"
#include "../Filesystem/Path.hpp"
#include "../Math/Geometry/Rect.hpp"
#include "../Math/Size.hpp"
#include "../UI/Controllers/SelectionController.hpp"
#include "../Function.hpp"

//Forward declarations
namespace ACStdLib
{
	namespace UI
	{
		class Widget;
	}
}

namespace _ACStdLib_internal
{
	enum class WindowBackendType
	{
		ComboBox,
		GroupBox,
		PushButton,
		SearchBox,
		TreeView,
		Window
	};

	class WindowBackend
	{
	public:
		virtual ~WindowBackend()
		{
		}

		//Abstract
		virtual void ClearView() const = 0;
		virtual WindowBackend *CreateChildBackend(_ACStdLib_internal::WindowBackendType type, ACStdLib::UI::Widget *widget) const = 0;
		virtual ACStdLib::Size GetSize() const = 0;
		virtual ACStdLib::Size GetSizeHint() const = 0;
		virtual void Select(ACStdLib::UI::ControllerIndex &controllerIndex) const = 0;
		virtual ACStdLib::Path SelectExistingDirectory(const ACStdLib::String &title, const ACStdLib::Function<bool(ACStdLib::Path &)> callback) const = 0;
		virtual void SetBounds(const ACStdLib::Rect &area) = 0;
		virtual void SetEnabled(bool enable) const = 0;
		virtual void SetHint(const ACStdLib::String &text) const = 0;
		virtual void SetText(const ACStdLib::String &text) = 0;
		virtual void Show(bool visible) = 0;
		virtual void ShowInformationBox(const ACStdLib::String &title, const ACStdLib::String &message) const = 0;
		virtual void UpdateSelection(ACStdLib::UI::SelectionController &selectionController) const = 0;
	};
}