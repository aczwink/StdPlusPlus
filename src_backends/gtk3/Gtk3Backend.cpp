/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include "Gtk3Backend.hpp"
//Global
#include <gtk/gtk.h>
//Local
#include "UI/Gtk3WindowBackend.hpp"
#include "UI/Gtk3TreeViewBackend.hpp"
#include "UI/GtkEventSource.hpp"
#include "Gtk3OpenGLBackend.hpp"
#include "UI/Gtk3RenderTargetWidgetBackend.hpp"
#include "UI/Gtk3GroupBoxBackend.hpp"
#include "UI/Gtk3SliderBackend.hpp"
#include "UI/Gtk3SelectBoxBackend.hpp"
#include "UI/Gtk3PushButtonBackend.hpp"
#include "UI/Gtk3LabelBackend.hpp"
#include "UI/Containers/Gtk3TabContainerBackend.hpp"
#include "UI/Views/Gtk3ListViewBackend.hpp"
#include "UI/Controls/Gtk3ProgressBarBackend.hpp"
#include "UI/Menu/Gtk3MenuBackend.hpp"
#include "UI/Transfer/Gtk3ClipboardBackend.hpp"
#include "UI/Controls/Gtk3TextEditBackend.hpp"
#include "UI/Controls/Gtk3CheckBoxBackend.hpp"
#include "UI/Menu/Gtk3MenuBarBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
Gtk3Backend::Gtk3Backend()
{
    //register render backends
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
    Gtk3OpenGLBackend *gtkOpenGLBackend = new Gtk3OpenGLBackend;
    this->renderBackends.RegisterBackend(gtkOpenGLBackend, 0);
#endif
}

//Public methods
CheckBoxBackend* Gtk3Backend::CreateCheckBoxBackend(UI::CheckBox& checkBox)
{
    return new Gtk3CheckBoxBackend(*this, checkBox);
}

ClipboardBackend *Gtk3Backend::CreateClipboardBackend()
{
    return new Gtk3ClipboardBackend;
}

EventHandling::EventSource *Gtk3Backend::CreateEventSource()
{
    return new GtkEventSource;
}

GroupBoxBackend *Gtk3Backend::CreateGroupBoxBackend(UI::GroupBox& groupBox)
{
    return new Gtk3GroupBoxBackend(*this, groupBox);
}

LabelBackend* Gtk3Backend::CreateLabelBackend(UI::Label& label)
{
	return new Gtk3LabelBackend(*this, label);
}

ViewBackend *Gtk3Backend::CreateListViewBackend(UI::ListView &listView)
{
    return new Gtk3ListViewBackend(*this, listView);
}

MenuBackend* Gtk3Backend::CreateMenuBackend(UI::Menu& menu)
{
    return new Gtk3MenuBackend(*this, menu);
}

MenuBarBackend *Gtk3Backend::CreateMenuBarBackend(UI::MenuBar* menuBar)
{
    return new Gtk3MenuBarBackend(*this, *menuBar);
}

ProgressBarBackend *Gtk3Backend::CreateProgressBarBackend(UI::ProgressBar &progressBar)
{
    return new Gtk3ProgressBarBackend(*this, progressBar);
}

PushButtonBackend *Gtk3Backend::CreatePushButtonBackend(UI::PushButton& pushButton)
{
    return new Gtk3PushButtonBackend(*this, pushButton);
}

WidgetBackend *Gtk3Backend::CreateRenderTargetWidgetBackend(UI::RenderTargetWidget& renderTargetWidget)
{
	return new Gtk3RenderTargetWidgetBackend(*this, renderTargetWidget);
}

ViewBackend *Gtk3Backend::CreateSelectBoxBackend(UI::SelectBox &selectBox)
{
    return new Gtk3SelectBoxBackend(*this, selectBox);
}

SliderBackend *Gtk3Backend::CreateSliderBackend(UI::Slider& slider)
{
    return new Gtk3SliderBackend(*this, slider);
}

TabContainerBackend *Gtk3Backend::CreateTabContainerBackend(UI::TabContainer &tabContainer)
{
    return new Gtk3TabContainerBackend(*this, tabContainer);
}

TextEditBackend *Gtk3Backend::CreateTextEditBackend(UI::TextEdit &textEdit)
{
    return new Gtk3TextEditBackend(*this, textEdit);
}

ViewBackend *Gtk3Backend::CreateTreeViewBackend(UI::TreeView &treeView)
{
    return new Gtk3TreeViewBackend(*this, treeView);
}

WindowBackend *Gtk3Backend::CreateWindowBackend(UI::Window *window)
{
    return new Gtk3WindowBackend(*this, *window);
}

void Gtk3Backend::Load()
{
    gtk_init(nullptr, nullptr);
}

//NOT IMPLEMENTED
_stdxx_::DrawableWidgetBackend *_stdxx_::Gtk3Backend::CreateDrawableWidgetBackend(StdXX::UI::Widget &widget) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::HeaderViewBackend *_stdxx_::Gtk3Backend::CreateHeaderViewBackend(StdXX::UI::HeaderView &headerView) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::ContentAreaWidgetBackend *_stdxx_::Gtk3Backend::CreateScrollAreaBackend(StdXX::UI::ScrollArea *scrollArea) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::WidgetBackend *_stdxx_::Gtk3Backend::CreateSearchBoxBackend(StdXX::UI::SearchBox &searchBox) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::SpinBoxBackend *_stdxx_::Gtk3Backend::CreateSpinBoxBackend(StdXX::UI::SpinBox *spinBox) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::ViewBackend *_stdxx_::Gtk3Backend::CreateTableViewBackend(StdXX::UI::TableView &treeView) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}