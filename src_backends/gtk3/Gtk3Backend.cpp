/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
EventSource *Gtk3Backend::CreateEventSource()
{
    return new GtkEventSource;
}

MenuBarBackend *Gtk3Backend::CreateMenuBarBackend(UI::MenuBar *menuBar)
{
    return nullptr;
}

WidgetBackend *Gtk3Backend::CreateRenderTargetWidgetBackend(UI::RenderTargetWidget& renderTargetWidget)
{
	return new Gtk3RenderTargetWidgetBackend(*this, renderTargetWidget);
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
_stdxx_::CheckBoxBackend *_stdxx_::Gtk3Backend::CreateCheckBoxBackend(StdXX::UI::CheckBox *checkBox) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::DrawableWidgetBackend *_stdxx_::Gtk3Backend::CreateDrawableWidgetBackend(StdXX::UI::Widget &widget) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::GroupBoxBackend *_stdxx_::Gtk3Backend::CreateGroupBoxBackend(StdXX::UI::GroupBox *groupBox) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::HeaderViewBackend *_stdxx_::Gtk3Backend::CreateHeaderViewBackend(StdXX::UI::HeaderView &headerView) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::LabelBackend *_stdxx_::Gtk3Backend::CreateLabelBackend(StdXX::UI::Label *label) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::MenuBackend *_stdxx_::Gtk3Backend::CreateMenuBackend(StdXX::UI::Menu *menu) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::PushButtonBackend *_stdxx_::Gtk3Backend::CreatePushButtonBackend(StdXX::UI::PushButton *pushButton) {
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

_stdxx_::ViewBackend *_stdxx_::Gtk3Backend::CreateSelectBoxBackend(StdXX::UI::SelectBox &selectBox) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return nullptr;
}

_stdxx_::SliderBackend *_stdxx_::Gtk3Backend::CreateSliderBackend(StdXX::UI::Slider *slider) {
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