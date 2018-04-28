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
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UIBackend.hpp>
#include <Std++/Time/Timer.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Global variables
EventQueue *g_globalEventQueue = nullptr;

//Constructor
EventQueue::EventQueue()
		: quit(false)
{
}

//Destructor
EventQueue::~EventQueue()
{
	for(EventSource *source : this->sources)
		delete source;
}

//Public methods
bool EventQueue::ProcessEvents(bool block)
{
	if(block)
	{
		this->DispatchPendingEvents();

		while(!this->quit)
		{
			this->WaitForEvents();
			this->DispatchPendingEvents();
		}
	}
	else
	{
		this->DispatchPendingEvents();
	}

	return !this->quit;
}

//Private methods
void EventQueue::DispatchPendingEvents()
{
	for(EventSource *const& source : this->sources)
	{
		source->DispatchPendingEvents();
	}
}

void EventQueue::WaitForEvents()
{
	uint64 minWaitTime = Natural<uint64>::Max();

	for(const EventSource *const& source : this->sources)
	{
		uint64 tmp = source->GetMaxTimeout();
		minWaitTime = MIN(minWaitTime, tmp);
	}

	if(minWaitTime == 0)
		return;

	this->System_WaitForEvents(minWaitTime);
}