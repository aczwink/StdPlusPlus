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
#include "GtkEventSource.hpp"
//Local
#include <Std++/Streams/StdOut.hpp>
#include <Std++/UI/Views/ComboBox.hpp>
#include <Std++/UI/Views/TreeView.hpp>
#include <Std++/UI/Controls/Slider.hpp>
#include "Gtk.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Global variables
bool g_ignoreEvent = false;
//Local variables
static GtkEventSource *l_gtkEvtSrc = nullptr;

//Constructor
GtkEventSource::GtkEventSource()
{
	this->context = g_main_context_default();

	l_gtkEvtSrc = this;
}

//Public methods
void GtkEventSource::DispatchPendingEvents()
{
	if(g_main_context_acquire(this->context))
	{
		if(this->pollFds.IsEmpty())
            g_main_context_check(this->context, G_PRIORITY_HIGH, nullptr, 0);
        else
            g_main_context_check(this->context, G_PRIORITY_HIGH, &this->pollFds[0], this->pollFds.GetNumberOfElements());
		g_main_context_dispatch(this->context);
		g_main_context_release(this->context);
	}
	else
	{
		NOT_IMPLEMENTED_ERROR;
	}
}

void GtkEventSource::VisitWaitObjects(const Function<void(_stdpp::WaitObjHandle, bool)> &visitFunc) const
{
	for(const GPollFD &gPollFD : this->pollFds)
	{
		_stdpp::WaitObjHandle waitObjHandle;
		waitObjHandle.fd = gPollFD.fd;
		visitFunc(Move(waitObjHandle), (gPollFD.events & G_IO_IN) != 0);
	}
}

uint64 GtkEventSource::GetMaxTimeout() const
{
	if(g_main_context_acquire(this->context))
	{
		gint maxPriority;
		g_main_context_prepare(this->context, &maxPriority);

		uint32 nEventObjects = 0;
		gint timeOut; //in milliseconds
        GPollFD *pfds;
		do
		{
			this->pollFds.Resize(nEventObjects);
            if(this->pollFds.IsEmpty())
                pfds = nullptr;
            else
                pfds = &this->pollFds[0];
		}
		while((nEventObjects = static_cast<uint32>(g_main_context_query(this->context, maxPriority, &timeOut, pfds, this->pollFds.GetNumberOfElements()))) > this->pollFds.GetNumberOfElements());
		this->pollFds.Resize(nEventObjects); //just set correct size, in case nEventObjects got smaller during last call

		g_main_context_release(this->context);

		return static_cast<uint64>(timeOut * 1000000);
	}
	else
	{
		NOT_IMPLEMENTED_ERROR;
	}

	return Natural<uint64>::Max();
}

//Public slots
bool GtkEventSource::ButtonSlot(GtkWidget *gtkWidget, GdkEventButton *event, gpointer user_data)
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
			l_gtkEvtSrc->DispatchMouseButtonPressed(*widget, button, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));
			break;
		case GDK_BUTTON_RELEASE:
			l_gtkEvtSrc->DispatchMouseButtonReleased(*widget, button, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));
			break;
		default:
			g_ignoreEvent = true;
	}

	return !g_ignoreEvent;
}

void GtkEventSource::ChangedSlot(GtkComboBox *gtkComboBox, gpointer user_data)
{
	l_gtkEvtSrc->DispatchSelectionChangedEvent(*(ComboBox *)user_data);
}

void GtkEventSource::CheckResizeSlot(GtkContainer *container, gpointer user_data)
{
	l_gtkEvtSrc->DispatchResizedEvent(*(Window *)user_data);
}

void GtkEventSource::ClickedSlot(GtkButton *button, gpointer user_data)
{
	if(GTK_IS_RADIO_BUTTON(button))
		l_gtkEvtSrc->DispatchActivatedEvent(*(RadioButton *) user_data);
	else
		l_gtkEvtSrc->DispatchActivatedEvent(*(PushButton *) user_data);
}

bool GtkEventSource::CloseSlot(GtkWidget *pWidget, GdkEvent *pEvent)
{
	g_ignoreEvent = false;

	l_gtkEvtSrc->DispatchCloseEvent(*(Window *)WIDGET_FROM_GTK(pWidget));

	return !g_ignoreEvent;
}

bool GtkEventSource::MouseMotionSlot(GtkWidget *gtkWidget, GdkEventMotion *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)WIDGET_FROM_GTK(gtkWidget);

	l_gtkEvtSrc->DispatchMouseMovedEvent(*widget, Point((int32) event->x, (int32) (widget->GetSize().height - event->y)));

	return !g_ignoreEvent;
}

bool GtkEventSource::PaintSlot(GtkGLArea *glArea, GdkGLContext *context, gpointer user_data)
{
	g_ignoreEvent = false;

	l_gtkEvtSrc->DispatchPaintEvent(*(Widget *)user_data);

	return !g_ignoreEvent;
}

bool GtkEventSource::ScrollSlot(GtkWidget *gtkWidget, GdkEventScroll *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)user_data;

	switch(event->direction)
	{
		case GDK_SCROLL_UP:
			l_gtkEvtSrc->DispatchMouseWheelEvent(*widget, 1);
			break;
		case GDK_SCROLL_DOWN:
			l_gtkEvtSrc->DispatchMouseWheelEvent(*widget, -1);
			break;
	}

	return !g_ignoreEvent;
}

void GtkEventSource::SizeAllocateSlot(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data)
{
	Rect rc;
	if(gtk_widget_get_parent_window(widget) == nullptr)
	{
		gint x, y;
		gtk_window_get_position(GTK_WINDOW(widget), &x, &y);
		rc.x() = x;
		rc.y() = y;
	}
	else
	{
		rc.x() = allocation->x;
		rc.y() = allocation->y;
	}
	rc.width() = static_cast<uint16>(allocation->width);
	rc.height() = static_cast<uint16>(allocation->height);
	l_gtkEvtSrc->DispatchResizingEvent(*(Widget *)user_data, rc);
}

void GtkEventSource::ToggledSlot(GtkToggleButton *toggleButton, gpointer user_data)
{
	l_gtkEvtSrc->DispatchToggledEvent(*(CheckBox *)user_data);
}

void GtkEventSource::TreeSelectionSlot(GtkTreeSelection *treeSelection, gpointer user_data)
{
	l_gtkEvtSrc->DispatchSelectionChangedEvent(*(TreeView *)user_data);
}

void GtkEventSource::ValueChangedSlot(GtkRange *range, gpointer user_data)
{
	Slider *slider = (Slider *) user_data;
	if(slider->onValueChangedHandler)
		slider->onValueChangedHandler();
}
