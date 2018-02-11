/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
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
#include <ACStdLib/UI/Window.hpp>
//Global
#include <gtk/gtk.h>
//Local
#include <ACStdLib/UI/Menu/CMenuBar.h>
#include "GtkEventQueue.hpp"
#include "Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS PRIVATE_DATA(this)

//Eventhandlers
void Window::OnPaint()
{
    NOT_IMPLEMENTED_ERROR;
}

//Private methods
void Window::MenuBarChangeOS()
{
    //gtk_container_add(GTK_CONTAINER(THIS->childAreaWidget), (GtkWidget *)this->pMenuBar->pOSHandle);
    //gtk_box_pack_start(GTK_BOX(THIS->childAreaWidget), (GtkWidget *)this->pMenuBar->backend, FALSE, FALSE, 0);
}

//Public methods
void Window::Maximize()
{
	gtk_window_maximize(GTK_WINDOW(THIS->widget));
	gtk_widget_show(THIS->widget);
}

void Window::ShowErrorBox(const OldString &title, const OldString &message)
{
	UTF8String messageUTF8 = message.GetUTF16();
	GtkWidget *widget = gtk_message_dialog_new(GTK_WINDOW(THIS->widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (const gchar *) messageUTF8.GetC_Str());

	UTF8String titleUTF8 = title.GetUTF16();
	gtk_window_set_title(GTK_WINDOW(widget), (const gchar *) titleUTF8.GetC_Str());

	gtk_dialog_run(GTK_DIALOG(widget));

	gtk_widget_destroy(widget);
}