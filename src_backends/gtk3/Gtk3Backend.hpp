/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#ifdef _STDPLUSPLUS_BACKEND_GTK3
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>

namespace _stdxx_
{
    class Gtk3Backend : public StdXX::UIBackend
    {
    public:
        //Constructor
        Gtk3Backend();

        //Methods
        CheckBoxBackend *CreateCheckBoxBackend(StdXX::UI::CheckBox& checkBox) override;
        ClipboardBackend *CreateClipboardBackend() override;
		DialogBackend* CreateDialogBackend(StdXX::UI::Dialog& dialog) override;
		DrawableWidgetBackend *CreateDrawableWidgetBackend(StdXX::UI::Widget &widget) override;
        StdXX::EventHandling::EventSource *CreateEventSource() override;
        GroupBoxBackend *CreateGroupBoxBackend(StdXX::UI::GroupBox& groupBox) override;
		LabelBackend *CreateLabelBackend(StdXX::UI::Label& label) override;
        ViewBackend *CreateListViewBackend(StdXX::UI::ListView &listView) override;
        MenuBackend *CreateMenuBackend(StdXX::UI::Menu& menu) override;
        MenuBarBackend *CreateMenuBarBackend(StdXX::UI::MenuBar *menuBar) override;
        ProgressBarBackend *CreateProgressBarBackend(StdXX::UI::ProgressBar &progressBar) override;
        PushButtonBackend *CreatePushButtonBackend(StdXX::UI::PushButton& pushButton) override;
	    WidgetBackend *CreateRenderTargetWidgetBackend(StdXX::UI::RenderTargetWidget& renderTargetWidget) override;
        ViewBackend *CreateSelectBoxBackend(StdXX::UI::SelectBox &selectBox) override;
        SliderBackend *CreateSliderBackend(StdXX::UI::Slider& slider) override;
        TabContainerBackend *CreateTabContainerBackend(StdXX::UI::TabContainer &tabContainer) override;
        TextEditBackend *CreateTextEditBackend(StdXX::UI::TextEdit &textEdit) override;
        ViewBackend *CreateTreeViewBackend(StdXX::UI::TreeView &treeView) override;
        WindowBackend *CreateWindowBackend(StdXX::UI::Window *window) override;
	    void Load() override;

        //NOT IMPLEMENTED
        HeaderViewBackend *CreateHeaderViewBackend(StdXX::UI::HeaderView &headerView) override;
        ContentAreaWidgetBackend *CreateScrollAreaBackend(StdXX::UI::ScrollArea *scrollArea) override;
        WidgetBackend *CreateSearchBoxBackend(StdXX::UI::SearchBox &searchBox) override;
        SpinBoxBackend *CreateSpinBoxBackend(StdXX::UI::SpinBox *spinBox) override;
        ViewBackend *CreateTableViewBackend(StdXX::UI::TableView &treeView) override;
	};
}
#endif