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
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/_Backends/UI/WidgetContainerBackend.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "CommCtrlWidgetBackend.hpp"

namespace _stdxx_
{
	class CommCtrlContainerBackend : virtual public WidgetContainerBackend, virtual public CommCtrlWidgetBackend
	{
	public:
		//Constructor
		inline CommCtrlContainerBackend(StdXX::UIBackend *uiBackend, StdXX::UI::WidgetContainer *container)
			: WidgetContainerBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend),
			container(container)
		{
		}

		//Methods
		void AddChild(StdXX::UI::Widget * widget) override;
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void IgnoreEvent() override;
		void Repaint() override;
		void ResetView() const override;
		void Select(StdXX::UI::ControllerIndex & controllerIndex) const override;
		void SetEditable(bool enable) const override;
		void SetEnabled(bool enable) override;
		void SetHint(const StdXX::String & text) const override;
		void UpdateSelection(StdXX::UI::SelectionController & selectionController) const override;

	private:
		//Members
		StdXX::UI::WidgetContainer *container;
	};
}