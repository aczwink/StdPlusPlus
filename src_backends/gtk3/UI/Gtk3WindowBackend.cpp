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
#include "Gtk3WindowBackend.hpp"
//Local
#include <Std++/UI/Events/WindowResizedEvent.hpp>
#include <Std++/UI/Window.hpp>
#include "Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Local functions
static gboolean CloseSlot(GtkWidget* gtkWidget, GdkEvent* gdkEvent, gpointer user_data)
{
	Gtk3WindowBackend* backend = static_cast<Gtk3WindowBackend *>(user_data);

	Event event(EventType::WindowShouldBeClosed);
	backend->GetWidget().Event(event);

	return event.WasAccepted();
}

static void SizeAllocateSlot(GtkWidget* gtkWidget, GdkRectangle *allocation, gpointer user_data)
{
	Gtk3WindowBackend* backend = static_cast<Gtk3WindowBackend *>(user_data);

	Math::SizeD newSize(allocation->width, allocation->height);
	WindowResizedEvent event(newSize);
	backend->GetWidget().Event(event);
}




//Constructor
Gtk3WindowBackend::Gtk3WindowBackend(UIBackend& backend, Window& window) : WindowBackend(backend), WidgetContainerBackend(backend),
	Gtk3WidgetBackend(backend, gtk_window_new(GTK_WINDOW_TOPLEVEL)), WidgetBackend(backend), window(window)
{
	this->wasVisibleBefore = false;

	GtkWidget* gtkWidget = this->GetGtkWidget();

	g_object_set_data(G_OBJECT(gtkWidget), u8"Std++", &window);

	g_signal_connect(gtkWidget, u8"delete-event", G_CALLBACK(CloseSlot), this);
	g_signal_connect(gtkWidget, u8"size-allocate", G_CALLBACK(SizeAllocateSlot), this);
}

//Public methods
void Gtk3WindowBackend::AddChild(Widget *widget)
{
	Gtk3WidgetBackend* backend = dynamic_cast<Gtk3WidgetBackend *>(widget->_GetBackend());
	gtk_container_add(GTK_CONTAINER(this->GetGtkWidget()), backend->GetGtkWidget());
}

WidgetContainerBackend *Gtk3WindowBackend::CreateContentAreaBackend(CompositeWidget &widget)
{
	Gtk3RedirectContainer* container = new Gtk3RedirectContainer(this->GetUIBackend(), widget);
	gtk_container_add(GTK_CONTAINER(this->GetGtkWidget()), container->GetGtkWidget());
    return container;
}

Math::RectD Gtk3WindowBackend::GetContentAreaBounds() const
{
	GtkBin* gtkBin = GTK_BIN(this->GetGtkWidget());
	GtkWidget* gtkChild = gtk_bin_get_child(gtkBin);

	GtkAllocation alloc;
	gtk_widget_get_allocation(gtkChild, &alloc);

	return Math::RectD(alloc.x, alloc.y, alloc.width, alloc.height);
}

Widget &Gtk3WindowBackend::GetWidget()
{
    return this->window;
}

const Widget &Gtk3WindowBackend::GetWidget() const
{
    return this->window;
}

void Gtk3WindowBackend::Maximize()
{
	gtk_window_maximize(GTK_WINDOW(this->GetGtkWidget()));
}

void Gtk3WindowBackend::SetTitle(const String &title)
{
    gtk_window_set_title(GTK_WINDOW(this->GetGtkWidget()), reinterpret_cast<const gchar *>(title.ToUTF8().GetRawZeroTerminatedData()));
}

void Gtk3WindowBackend::Show(bool visible)
{
	if(visible && (!this->wasVisibleBefore))
	{
		this->wasVisibleBefore = true;

		Math::SizeD size = this->GetWidget().GetSizeHint();

		//add titlebar
		/*int min, nat;
		gtk_widget_get_preferred_height(this->headerBar, &min, &nat);
		size.height += Math::Max(min, nat);*/

		gtk_window_set_default_size(GTK_WINDOW(this->GetGtkWidget()), size.width, size.height);
	}
	Gtk3WidgetBackend::Show(visible);
}

//NOT IMPLEMENTED
void _stdxx_::Gtk3WindowBackend::IgnoreEvent() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::Repaint() {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::SetBounds(const StdXX::Math::RectD &bounds) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::SetEditable(bool enable) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

StdXX::Path _stdxx_::Gtk3WindowBackend::SelectExistingDirectory(const StdXX::String &title, const StdXX::Function<bool(
        StdXX::Path &)> callback) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
    return StdXX::Path();
}

void _stdxx_::Gtk3WindowBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, _stdxx_::MenuBarBackend *menuBarBackend) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path Gtk3WindowBackend::SelectExistingFile(const StdXX::String &title,
                                           const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>> &filters,
                                           const StdXX::Path &initialPath) const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Path();
}
