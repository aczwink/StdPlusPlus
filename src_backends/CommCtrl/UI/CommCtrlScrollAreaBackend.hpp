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
//Local
#include <Std++/_Backends/UI/ContentAreaWidgetBackend.hpp>
#include <Std++/UI/Containers/ScrollArea.hpp>
#include "CommCtrlContainerBackend.hpp"
#include "Win32Window.hpp"
#include "../Import.hpp"

namespace _stdxx_
{
	class CommCtrlScrollAreaBackend : public ContentAreaWidgetBackend, public CommCtrlContainerBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlScrollAreaBackend(StdXX::UIBackend *uiBackend, StdXX::UI::ScrollArea *scrollArea)
			: ContentAreaWidgetBackend(uiBackend), CommCtrlContainerBackend(uiBackend, scrollArea), WidgetContainerBackend(uiBackend),
			CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, STDPLUSPLUS_WIN_WNDCLASS, WS_CLIPCHILDREN),
			scrollArea(scrollArea)
		{
		}

		//Methods
		_stdxx_::WidgetContainerBackend * CreateContentAreaBackend(StdXX::UI::CompositeWidget & widget) override;
		StdXX::Math::RectD GetContentAreaBounds() const override;
		StdXX::Math::SizeD GetSizeHint() const override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;

		//NOt implemented:
		void Repaint() override;
		void SetEditable(bool enable) const override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String & text) const override;
		void Show(bool visible) override;
		void IgnoreEvent() override;
		void ResetView() const override;
		void Select(StdXX::UI::ControllerIndex & controllerIndex) const override;
		void UpdateSelection(StdXX::UI::SelectionController & selectionController) const override;

	private:
		//Members
		StdXX::UI::ScrollArea *scrollArea;
	};
}