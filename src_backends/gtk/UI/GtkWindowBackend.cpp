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
//Class header
#include "GtkWindowBackend.hpp"
//Local
#include "_RedirectGtkContainer.h"
#include "GtkEventQueue.hpp"
//Namespaces
using namespace _ACStdLib_internal;

struct FileChooserData
{
	const Function<bool(Path &)> *callback;
	GtkWidget *acceptButton;
};

static void SelectionChanged(GtkFileChooser *fileChooser, gpointer user_data)
{
	bool accept = false;
	FileChooserData *fcd = static_cast<FileChooserData *>(user_data);
	char *fileName = gtk_file_chooser_get_filename(fileChooser);
	if(fileName)
	{
		Path path = String(fileName);
		accept = (*fcd->callback)(path);
		g_free(fileName);
	}

	gtk_widget_set_sensitive(fcd->acceptButton, accept);
};

//Constructor
GtkWindowBackend::GtkWindowBackend(_ACStdLib_internal::WindowBackendType type, Widget *widget)
{
	bool isContainer = true;
	switch(type)
	{
		case _ACStdLib_internal::WindowBackendType::Window:
		{
			this->gtkWidget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

			g_signal_connect(this->gtkWidget, "check-resize", G_CALLBACK(GtkEventQueue::CheckResizeSlot), widget);
			g_signal_connect(this->gtkWidget, "delete-event", G_CALLBACK(GtkEventQueue::CloseSlot), nullptr);
			g_signal_connect(this->gtkWidget, "destroy", G_CALLBACK(GtkEventQueue::DestroySlot), widget);
		}
		break;
	}

	g_object_set_data(G_OBJECT(this->gtkWidget), "ACStdLib", widget);

	this->childAreaWidget = nullptr;
	if(isContainer)
	{
		this->childAreaWidget = redirect_container_new();
		g_object_set_data(G_OBJECT(this->childAreaWidget), "ACStdLib", widget);
		gtk_container_add(GTK_CONTAINER(this->gtkWidget), this->childAreaWidget);
		gtk_widget_show(this->childAreaWidget); //default to show
	}

	gtk_window_set_position(GTK_WINDOW(this->gtkWidget), GTK_WIN_POS_CENTER);
}

//Destructor
GtkWindowBackend::~GtkWindowBackend()
{
	if(this->childAreaWidget)
		gtk_widget_destroy(GTK_WIDGET(this->childAreaWidget));

	gtk_widget_destroy(GTK_WIDGET(this->gtkWidget));
}

//Public methods
Size GtkWindowBackend::GetSize() const
{
	GtkAllocation alloc;

	gtk_widget_get_allocation(this->gtkWidget, &alloc);

	return Size((uint16)alloc.width, (uint16)alloc.height);
}

Path GtkWindowBackend::SelectExistingDirectory(const String &title, const Function<bool(Path &)> callback) const
{
	GtkWidget *fileChooserDialog = gtk_file_chooser_dialog_new((gchar *)title.ToUTF8().GetRawZeroTerminatedData(), GTK_WINDOW(this->gtkWidget), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "Cancel", GTK_RESPONSE_CANCEL, "Select", GTK_RESPONSE_ACCEPT, nullptr);

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

void GtkWindowBackend::SetBounds(const ACStdLib::Rect &area)
{
	if(GTK_IS_WINDOW(this->gtkWidget))
		gtk_window_set_default_size(GTK_WINDOW(this->gtkWidget), area.width(), area.height());
	else
		gtk_widget_queue_resize(this->gtkWidget);
}

void GtkWindowBackend::Show(bool visible)
{
	if(visible)
		gtk_widget_show(this->gtkWidget);
	else
		gtk_widget_hide(this->gtkWidget);
}

void GtkWindowBackend::ShowInformationBox(const String &title, const String &message) const
{
	GtkWidget *widget = gtk_message_dialog_new(GTK_WINDOW(this->gtkWidget), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, (const gchar *) message.ToUTF8().GetRawZeroTerminatedData());
	gtk_window_set_title(GTK_WINDOW(widget), (const gchar *) title.ToUTF8().GetRawZeroTerminatedData());
	gtk_dialog_run(GTK_DIALOG(widget));
	gtk_widget_destroy(widget);
}