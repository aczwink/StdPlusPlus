/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "GtkEventQueue.hpp"
//Local
#include <Std++/Streams/CStdOut.h>
#include "Gtk.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Global variables
bool g_ignoreEvent = false;

//Public functions
bool GtkEventQueue::ButtonSlot(GtkWidget *gtkWidget, GdkEventButton *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)WIDGET_FROM_GTK(gtkWidget);
	MouseButton button;

	switch(event->button)
	{
		case 1:
			button = MouseButton::Left;
			break;
		case 3:
			button = MouseButton::Right;
			break;
		default:
			return FALSE;
	}

	switch(event->type)
	{
		case GDK_BUTTON_PRESS:
			EventQueue::DispatchMouseButtonPressed(*widget, button, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));
			break;
		case GDK_BUTTON_RELEASE:
			EventQueue::DispatchMouseButtonReleased(*widget, button, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));
			break;
		default:
			g_ignoreEvent = true;
	}

	return !g_ignoreEvent;
}

void GtkEventQueue::ChangedSlot(GtkComboBox *gtkComboBox, gpointer user_data)
{
	EventQueue::DispatchSelectionChangedEvent(*(ComboBox *)user_data);
}

void GtkEventQueue::CheckResizeSlot(GtkContainer *container, gpointer user_data)
{
	EventQueue::DispatchResizedEvent(*(Window *)user_data);
}

void GtkEventQueue::ClickedSlot(GtkButton *button, gpointer user_data)
{
	if(GTK_IS_RADIO_BUTTON(button))
		EventQueue::DispatchActivatedEvent(*(RadioButton *) user_data);
	else
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

bool GtkEventQueue::MouseMotionSlot(GtkWidget *gtkWidget, GdkEventMotion *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)WIDGET_FROM_GTK(gtkWidget);

	EventQueue::DispatchMouseMovedEvent(*widget, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));

	return !g_ignoreEvent;
}

bool GtkEventQueue::PaintSlot(GtkGLArea *glArea, GdkGLContext *context, gpointer user_data)
{
	g_ignoreEvent = false;

	EventQueue::DispatchPaintEvent(*(Widget *)user_data);

	return !g_ignoreEvent;
}

bool GtkEventQueue::ScrollSlot(GtkWidget *gtkWidget, GdkEventScroll *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)user_data;

	switch(event->direction)
	{
		case GDK_SCROLL_UP:
			EventQueue::DispatchMouseWheelEvent(*widget, 1);
			break;
		case GDK_SCROLL_DOWN:
			EventQueue::DispatchMouseWheelEvent(*widget, -1);
			break;
	}

	return !g_ignoreEvent;
}

void GtkEventQueue::SizeAllocateSlot(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data)
{
	EventQueue::DispatchResizedEvent(*(Widget *)user_data);
}

void GtkEventQueue::ToggledSlot(GtkToggleButton *toggleButton, gpointer user_data)
{
	EventQueue::DispatchToggledEvent(*(CheckBox *)user_data);
}

void GtkEventQueue::TreeSelectionSlot(GtkTreeSelection *treeSelection, gpointer user_data)
{
	EventQueue::DispatchSelectionChangedEvent(*(TreeView *)user_data);
}