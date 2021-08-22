/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Window.hpp>
#include "Gtk3RedirectContainer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

struct FileChooserData
{
	const Function<bool(FileSystem::Path &)> *callback;
	GtkWidget *acceptButton;
};

//Local functions
static gboolean CloseSlot(GtkWidget* gtkWidget, GdkEvent* gdkEvent, gpointer user_data)
{
	Gtk3WindowBackend* backend = static_cast<Gtk3WindowBackend *>(user_data);

	Event event(EventType::WindowShouldBeClosed);
	backend->GetWidget().Event(event);

	return event.WasAccepted();
}

static void SelectionChanged(GtkFileChooser *fileChooser, gpointer user_data)
{
	bool accept = false;
	FileChooserData *fcd = static_cast<FileChooserData *>(user_data);
	char *fileName = gtk_file_chooser_get_filename(fileChooser);
	if(fileName)
	{
		FileSystem::Path path = String(fileName);
		accept = (*fcd->callback)(path);
		g_free(fileName);
	}

	gtk_widget_set_sensitive(fcd->acceptButton, accept);
};


//Constructor
Gtk3WindowBackend::Gtk3WindowBackend(UIBackend& backend, Window& window) : WindowBackend(backend), WidgetContainerBackend(backend),
	Gtk3WidgetBackend(backend, gtk_window_new(GTK_WINDOW_TOPLEVEL)), WidgetBackend(backend), window(window)
{
	this->wasVisibleBefore = false;

	GtkWidget* gtkWidget = this->GetGtkWidget();

	g_signal_connect(gtkWidget, "delete-event", G_CALLBACK(CloseSlot), this);
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
	//according to doc, this should try to omit client side decorations, unfortunately position can't be known :S
	gint width, height;
    gtk_window_get_size(this->GetGtkWindow(), &width, &height);

    return Math::RectD(0, 0, width, height);
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

FileSystem::Path Gtk3WindowBackend::SelectExistingDirectory(const String &title, const Function<bool(FileSystem::Path &)> callback) const
{
	GtkWidget *fileChooserDialog = gtk_file_chooser_dialog_new((gchar *)title.ToUTF8().GetRawZeroTerminatedData(), GTK_WINDOW(this->GetGtkWidget()), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Cancel", GTK_RESPONSE_CANCEL, "Select", GTK_RESPONSE_ACCEPT, nullptr);

	FileChooserData fcd;
	fcd.callback = &callback;
	fcd.acceptButton = gtk_dialog_get_widget_for_response(GTK_DIALOG(fileChooserDialog), GTK_RESPONSE_ACCEPT);
	g_signal_connect(fileChooserDialog, "selection-changed", G_CALLBACK(SelectionChanged), (gpointer)&fcd);

	gint result = gtk_dialog_run(GTK_DIALOG(fileChooserDialog));
	String fileName;
	if(result == GTK_RESPONSE_ACCEPT)
	{
		char *pFileName;

		pFileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooserDialog));
		fileName = String::CopyRawString(pFileName);

		g_free(pFileName);
	}

	gtk_widget_destroy(fileChooserDialog);

	return fileName;
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

void _stdxx_::Gtk3WindowBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, _stdxx_::MenuBarBackend *menuBarBackend) {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::Gtk3WindowBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const {
    NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

FileSystem::Path Gtk3WindowBackend::SelectExistingFile(const StdXX::String &title,
                                           const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>> &filters,
                                           const StdXX::FileSystem::Path &initialPath) const {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::FileSystem::Path();
}
