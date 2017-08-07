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
#include <ACStdLib/UI/EventQueue.hpp>
//Global
#include <gtk/gtk.h>
#include <poll.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
//Definitions
#define THIS ((EventQueueInternal *)this->internal)
//Constants
const uint32 NUMBER_OF_SELF_EVENT_OBJECTS = 0; //other than gtk

/*
 * Because gtk uses POSIX file descriptors for waiting for events we're stuck to do that too.
 */
struct EventQueueInternal
{
	pollfd *eventObjects;
	uint32 nAllocatedEventObjects;
	GMainContext *context;
};

//Local functions
static void DoGTKEvents(EventQueueInternal *internal, uint64 minWaitTime_usec, bool block)
{
	if(g_main_context_acquire(internal->context))
	{
		gint maxPriority;
		g_main_context_prepare(internal->context, &maxPriority);

		gint timeOut;
		uint32 nUIEventObjects = 0, nEventObjects;
		while ((nUIEventObjects = static_cast<uint32>(g_main_context_query(internal->context, maxPriority, &timeOut, (GPollFD *) (internal->eventObjects), internal->nAllocatedEventObjects))) > internal->nAllocatedEventObjects)
		{
			nEventObjects = nUIEventObjects + NUMBER_OF_SELF_EVENT_OBJECTS;
			internal->eventObjects = static_cast<pollfd *>(MemRealloc(internal->eventObjects, nEventObjects * sizeof(pollfd)));
			internal->nAllocatedEventObjects = nEventObjects;
		}
		minWaitTime_usec = MIN(minWaitTime_usec, (uint64) timeOut * 1000);
		nEventObjects = nUIEventObjects + NUMBER_OF_SELF_EVENT_OBJECTS;

		//add other event objects

		if (block)
		{
			poll(internal->eventObjects, nEventObjects, minWaitTime_usec);
		}

		g_main_context_check(internal->context, maxPriority, (GPollFD *) (internal->eventObjects), nUIEventObjects);
		g_main_context_dispatch(internal->context);

		g_main_context_release(internal->context);
	}
	else
	{
		NOT_IMPLEMENTED_ERROR;
	}
}

//Constructor
EventQueue::EventQueue()
{
	this->internal = MemAlloc(sizeof(EventQueueInternal));
	THIS->eventObjects = nullptr;
	THIS->nAllocatedEventObjects = 0;
	THIS->context = g_main_context_default();
}

//Destructor
EventQueue::~EventQueue()
{
	MemFree(this->internal);
}

//Public methods
bool EventQueue::EventsPending()
{
	return g_main_context_pending(THIS->context) == TRUE;
}

//Private methods
void EventQueue::DispatchSystemEvents()
{
	DoGTKEvents(THIS, 0, false);
}

void EventQueue::WaitForEvents(uint64 minWaitTime_usec)
{
	if(true) //TODO: if this is the thread default event queue
		DoGTKEvents(THIS, minWaitTime_usec, true);
}