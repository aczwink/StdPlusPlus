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
#include <Std++/UI/EventQueue.hpp>
//Global
#include <gtk/gtk.h>
#include <poll.h>
#include <sys/eventfd.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;
//Definitions
#define THIS ((EventQueueInternal *)this->internal)
//Constants
const uint32 NUMBER_OF_SELF_EVENT_OBJECTS = 1; //other than gtk

/*
 * Because gtk uses POSIX file descriptors for waiting for events we're stuck to do that too.
 */
struct EventQueueInternal
{
	pollfd *eventObjects;
	uint32 nAllocatedEventObjects;
	GMainContext *context;
	int timerEventFd;
};

//Local functions
static void EnsureEnoughFDs(EventQueueInternal *internal, uint32 nRequiredEventObjects)
{
	internal->eventObjects = static_cast<pollfd *>(MemRealloc(internal->eventObjects, nRequiredEventObjects * sizeof(pollfd)));
	internal->nAllocatedEventObjects = nRequiredEventObjects;
}

static void DoGTKEvents(EventQueueInternal *internal, uint64 minWaitTime_usec, bool block)
{
	if(g_main_context_acquire(internal->context))
	{
		gint maxPriority;
		g_main_context_prepare(internal->context, &maxPriority);

		gint timeOut;
		uint32 nUIEventObjects = 0, nEventObjects;
		do
		{
			nEventObjects = nUIEventObjects + NUMBER_OF_SELF_EVENT_OBJECTS;
			EnsureEnoughFDs(internal, nEventObjects);
		}
		while ((nUIEventObjects = static_cast<uint32>(g_main_context_query(internal->context, maxPriority, &timeOut, (GPollFD *) (internal->eventObjects), internal->nAllocatedEventObjects))) > internal->nAllocatedEventObjects);
		minWaitTime_usec = MIN(minWaitTime_usec, (uint64) timeOut * 1000);

		uint32 currentFDIndex = nUIEventObjects;
		//add timer event object
		{
			pollfd &fd = internal->eventObjects[currentFDIndex++];

			fd.fd = internal->timerEventFd;
			fd.events = POLLIN;
			fd.revents = 0;
		}

		if (block)
		{
			timespec timeOut;

			timeOut.tv_sec = static_cast<__time_t>(minWaitTime_usec / 1000000);
			timeOut.tv_nsec = static_cast<__syscall_slong_t>((minWaitTime_usec % 1000000) * 1000);


			ppoll(internal->eventObjects, nEventObjects, &timeOut, nullptr);
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
	this->quit = false;
	this->internal = MemAlloc(sizeof(EventQueueInternal));
	THIS->eventObjects = nullptr;
	THIS->nAllocatedEventObjects = 0;
	THIS->context = g_main_context_default();
	THIS->timerEventFd = eventfd(0, 0);
}

//Destructor
EventQueue::~EventQueue()
{
	close(THIS->timerEventFd);
	if(THIS->eventObjects)
		MemFree(THIS->eventObjects);
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

void EventQueue::NotifyTimers()
{
	uint64 v = 1;
	write(THIS->timerEventFd, &v, sizeof(v));
}

uint64 EventQueue::QueryMonotonicClock()
{
	timespec current;

	clock_gettime(CLOCK_MONOTONIC, &current);

	return static_cast<uint64>(current.tv_sec * 1000000000 + current.tv_nsec);
}

void EventQueue::WaitForEvents(uint64 minWaitTime_usec)
{
	if(this == &EventQueue::GetGlobalQueue())
		DoGTKEvents(THIS, minWaitTime_usec, true);
}