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
#include <Std++/EventHandling/EventQueue.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/Time/Timer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::EventHandling;

//Constructor
EventQueue::EventQueue()
		: quit(false)
{
	this->System_Init();
}

//Destructor
EventQueue::~EventQueue()
{
	for(EventSource *source : this->sources)
		delete source;

	this->System_Shutdown();
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
		if(source->HasPendingEvents())
			source->DispatchPendingEvents();
	}
}

void EventQueue::WaitForEvents()
{
	uint64 minWaitTime = Unsigned<uint64>::Max();

	for(const EventSource *const& source : this->sources)
	{
		uint64 tmp = source->GetMaxTimeout();
		if(tmp == 0)
			return;
		minWaitTime = Math::Min(minWaitTime, tmp);
	}

	this->System_CollectWaitObjects();
	this->System_WaitForEvents(minWaitTime);
}