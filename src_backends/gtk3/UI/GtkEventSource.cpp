/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/EventHandling/WaitObjectManager.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::EventHandling;

//Constructor
GtkEventSource::GtkEventSource()
{
	this->context = g_main_context_default();
}

//Public methods
bool GtkEventSource::CheckWaitResults(const WaitResult& waitResults)
{
	const gboolean locked = g_main_context_acquire(this->context);
	ASSERT_EQUALS(TRUE, locked);

	for(uint32 i = 0; i < this->pollFds.GetNumberOfElements(); i++)
		this->pollFds[i].revents = waitResults.ResultAt(i);

	gboolean result;
	if(this->pollFds.IsEmpty())
		result = g_main_context_check(this->context, G_PRIORITY_HIGH, nullptr, 0);
	else
		result = g_main_context_check(this->context, G_PRIORITY_HIGH, &this->pollFds[0], this->pollFds.GetNumberOfElements());

	g_main_context_release(this->context);

	return result == TRUE;
}

void GtkEventSource::DispatchPendingEvents()
{
	const gboolean locked = g_main_context_acquire(this->context);
	ASSERT_EQUALS(TRUE, locked);

	g_main_context_dispatch(this->context);

	g_main_context_release(this->context);
}

bool GtkEventSource::HasPendingEvents() const
{
	return g_main_context_pending(this->context) == TRUE;
}

uint64 GtkEventSource::QueryWaitInfo(EventHandling::WaitObjectManager &waitObjectManager)
{
	const gboolean locked = g_main_context_acquire(this->context);
	ASSERT_EQUALS(TRUE, locked);

	gint maxPriority;
	if(g_main_context_prepare(this->context, &maxPriority) == TRUE)
	{
		g_main_context_release(this->context);
		return 0;
	}

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

	for(uint32 i = 0; i < nEventObjects; i++)
		waitObjectManager.Add(*this, this->pollFds[i].fd, this->pollFds[i].events);

	if(timeOut < 0)
		return Unsigned<uint64>::Max();
	return timeOut * 1000000_u64;
}

/*
//Public slots
void GtkEventSource::ChangedSlot(GtkComboBox *gtkComboBox, gpointer user_data)
{
    NOT_IMPLEMENTED_ERROR; //TODO: next line
    // l_gtkEvtSrc->DispatchSelectionChangedEvent(*(ComboBox *)user_data);
}

void GtkEventSource::CheckResizeSlot(GtkContainer *container, gpointer user_data)
{
    NOT_IMPLEMENTED_ERROR; //TODO: next line
	//l_gtkEvtSrc->DispatchResizedEvent(*(Window *)user_data);
}

void GtkEventSource::ClickedSlot(GtkButton *button, gpointer user_data)
{
    NOT_IMPLEMENTED_ERROR; //TODO: next line
    /*
	if(GTK_IS_RADIO_BUTTON(button))
		l_gtkEvtSrc->DispatchActivatedEvent(*(RadioButton *) user_data);
     *//*
}

void GtkEventSource::EmitResizingEvent(Widget &widget, const Math::RectD &newBounds)
{
    NOT_IMPLEMENTED_ERROR; //TODO: next line
	//l_gtkEvtSrc->DispatchResizingEvent(widget, newBounds);
}

bool GtkEventSource::ScrollSlot(GtkWidget *gtkWidget, GdkEventScroll *event, gpointer user_data)
{
	g_ignoreEvent = false;

	Widget *widget = (Widget *)user_data;

    NOT_IMPLEMENTED_ERROR; //TODO: next line
    /*
	switch(event->direction)
	{
		case GDK_SCROLL_UP:
			l_gtkEvtSrc->DispatchMouseWheelEvent(*widget, 1);
			break;
		case GDK_SCROLL_DOWN:
			l_gtkEvtSrc->DispatchMouseWheelEvent(*widget, -1);
			break;
	}*//*

	return !g_ignoreEvent;
}

void GtkEventSource::TreeSelectionSlot(GtkTreeSelection *treeSelection, gpointer user_data)
{
    NOT_IMPLEMENTED_ERROR; //TODO: next line
	//l_gtkEvtSrc->DispatchSelectionChangedEvent(*(TreeView *)user_data);
}*/