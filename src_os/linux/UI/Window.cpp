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
void Window::CreateOSWindow(const Rect &refRect)
{
	this->systemHandle = CreateWidgetContainerPrivateData(gtk_window_new(GTK_WINDOW_TOPLEVEL), this);

    gtk_window_set_position(GTK_WINDOW(THIS->widget), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(THIS->widget), refRect.width(), refRect.height());

	g_signal_connect(THIS->widget, "check-resize", G_CALLBACK(GtkEventQueue::CheckResizeSlot), this);
    g_signal_connect(THIS->widget, "delete-event", G_CALLBACK(GtkEventQueue::CloseSlot), nullptr);
    g_signal_connect(THIS->widget, "destroy", G_CALLBACK(GtkEventQueue::DestroySlot), this);
}

void Window::DestroyOSWindow()
{
	DestroyWidgetPrivateData(THIS);
}

void Window::MenuBarChangeOS()
{
    gtk_container_add(GTK_CONTAINER(THIS->childAreaWidget), (GtkWidget *)this->pMenuBar->pOSHandle);
    //gtk_box_pack_start(GTK_BOX(THIS->childAreaWidget), (GtkWidget *)this->pMenuBar->systemHandle, FALSE, FALSE, 0);
}

//Public methods
void Window::Maximize()
{
	gtk_window_maximize(GTK_WINDOW(THIS->widget));
	gtk_widget_show(THIS->widget);
}

void Window::SetTitle(const String &title)
{
	UTF8String text;

	text = title.GetUTF16();
	gtk_window_set_title(GTK_WINDOW(THIS->widget), (const gchar *)text.GetC_Str());
}