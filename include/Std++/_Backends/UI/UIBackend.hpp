/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Eventhandling/EventSource.hpp>
#include "Std++/_Backends/Backend.hpp"
#include "Std++/_Backends/BackendManager.hpp"
#include "Std++/_Backends/RenderBackend.hpp"

//Forward declarations
namespace _stdxx_
{
	class CheckBoxBackend;
	class ContentAreaWidgetBackend;
	class DrawableWidgetBackend;
	class GroupBoxBackend;
	class HeaderViewBackend;
	class LabelBackend;
	class MenuBackend;
	class MenuBarBackend;
	class PushButtonBackend;
	class SliderBackend;
	class SpinBoxBackend;
	class ViewBackend;
	class WidgetBackend;
	class WindowBackend;
}

namespace StdXX
{
	//Forward declarations
	class EventQueue;
	namespace UI
	{
		class CheckBox;
		class GroupBox;
		class HeaderView;
		class Label;
		class Menu;
		class MenuBar;
		class PushButton;
		class ScrollArea;
		class SearchBox;
		class SelectBox;
		class Slider;
		class SpinBox;
		class TableView;
		class TreeView;
		class Widget;
		class Window;
	}

	class UIBackend : public Backend
	{
	public:
		//Members
		BackendManager<RenderBackend> renderBackends;

		//Abstract
		virtual _stdxx_::CheckBoxBackend *CreateCheckBoxBackend(UI::CheckBox *checkBox) = 0;
		virtual _stdxx_::DrawableWidgetBackend* CreateDrawableWidgetBackend(UI::Widget& widget) = 0;
		virtual EventSource *CreateEventSource() = 0;
		virtual _stdxx_::GroupBoxBackend *CreateGroupBoxBackend(UI::GroupBox *groupBox) = 0;
		virtual _stdxx_::HeaderViewBackend* CreateHeaderViewBackend(UI::HeaderView& headerView) = 0;
		virtual _stdxx_::LabelBackend *CreateLabelBackend(UI::Label *label) = 0;
		virtual _stdxx_::MenuBackend *CreateMenuBackend(UI::Menu *menu) = 0;
		virtual _stdxx_::MenuBarBackend *CreateMenuBarBackend(UI::MenuBar *menuBar) = 0;
		virtual _stdxx_::PushButtonBackend *CreatePushButtonBackend(UI::PushButton *pushButton) = 0;
		virtual _stdxx_::WidgetBackend *CreateRenderTargetWidgetBackend(UI::RenderTargetWidget& renderTargetWidget) = 0;
		virtual _stdxx_::ContentAreaWidgetBackend* CreateScrollAreaBackend(UI::ScrollArea *scrollArea) = 0;
		virtual _stdxx_::WidgetBackend* CreateSearchBoxBackend(UI::SearchBox& searchBox) = 0;
		virtual _stdxx_::ViewBackend* CreateSelectBoxBackend(UI::SelectBox& selectBox) = 0;
		virtual _stdxx_::SliderBackend *CreateSliderBackend(UI::Slider *slider) = 0;
		virtual _stdxx_::SpinBoxBackend *CreateSpinBoxBackend(UI::SpinBox *spinBox) = 0;
		virtual _stdxx_::ViewBackend* CreateTableViewBackend(UI::TableView& treeView) = 0;
		virtual _stdxx_::ViewBackend* CreateTreeViewBackend(UI::TreeView& treeView) = 0;
		virtual _stdxx_::WindowBackend *CreateWindowBackend(UI::Window *window) = 0;

		//Methods
		BackendType GetType() const
		{
			return BackendType::UI;
		}
	};
}