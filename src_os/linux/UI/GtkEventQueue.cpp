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
#include "GtkEventQueue.hpp"
//Local
#include <ACStdLib/Streams/CStdOut.h>
#include "Gtk.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Global variables
bool g_ignoreEvent = false;

//Public functions
void GtkEventQueue::CheckResizeSlot(GtkContainer *container, gpointer user_data)
{
	EventQueue::DispatchResizedEvent(*(Window *)user_data);
}

void GtkEventQueue::ClickedSlot(GtkButton *button, gpointer user_data)
{
	EventQueue::DispatchActivatedEvent(*(PushButton *) user_data);
}

bool GtkEventQueue::CloseSlot(GtkWidget *pWidget, GdkEvent *pEvent)
{
	g_ignoreEvent = false;

    EventQueue::DispatchCloseEvent(*(Window *)WIDGET_FROM_GTK(pWidget));

	return !g_ignoreEvent;
}

void GtkEventQueue::DestroySlot(GtkWidget *pWidget, gpointer data)
{
    EventQueue::DispatchDestroyEvent(*(Window *)data);
}

bool GtkEventQueue::PaintSlot(GtkGLArea *glArea, GdkGLContext *context, gpointer user_data)
{
	g_ignoreEvent = false;

	EventQueue::DispatchPaintEvent(*(Widget *)user_data);

	return !g_ignoreEvent;
}