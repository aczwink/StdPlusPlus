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
#include <Std++/Eventhandling/EventQueue.hpp>
//Global
#include <gtk/gtk.h>
#include <sys/eventfd.h>
//Namespaces
using namespace StdPlusPlus;
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
	int timerEventFd;
};

//Constructor
EventQueue::EventQueue()
{
	THIS->timerEventFd = eventfd(0, 0);
}

//Destructor
EventQueue::~EventQueue()
{
	close(THIS->timerEventFd);
}

//Public methods
bool EventQueue::EventsPending()
{
	return g_main_context_pending(THIS->context) == TRUE;
}

//Private methods
void EventQueue::NotifyTimers()
{
	uint64 v = 1;
	write(THIS->timerEventFd, &v, sizeof(v));
}