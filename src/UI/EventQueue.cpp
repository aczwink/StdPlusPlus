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
	delete this->backend;
}

//Public methods
bool EventQueue::ProcessEvents(bool block)
{
	if(block)
	{
		while(!this->quit)
		{
			uint64 minWaitTime_usec = this->GetShortestTimerTimeOut();
			this->DispatchPendingEvents();
			if(this->backend)
				this->backend->WaitForEvents(minWaitTime_usec);
		}

		this->DispatchPendingEvents();
	}
	else
	{
		this->DispatchPendingEvents();
	}

	return !this->quit;
}

//Class functions
EventQueue &EventQueue::GetGlobalQueue()
{
	if(g_globalEventQueue == nullptr)
	{
		g_globalEventQueue = new EventQueue;
		g_globalEventQueue->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateEventQueueBackend(*g_globalEventQueue);
	}

	return *g_globalEventQueue;
}

//Private methods
void EventQueue::DispatchPendingEvents()
{
	this->DispatchTimers();
	if(this->backend)
		this->backend->DispatchPendingEvents();
}

void EventQueue::DispatchTimers()
{
	uint64 currentClock = this->clock.GetCurrentValue();
	while(!this->oneShotTimerQueue.IsEmpty())
	{
		if(currentClock >= this->oneShotTimerQueue.GetFirstPriority())
		{
			this->oneShotTimerQueue.PopFirst()->timedOutCallback();
		}
		else
		{
			break;
		}
	}
}

uint64 EventQueue::GetShortestTimerTimeOut()
{
	if(!this->oneShotTimerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->oneShotTimerQueue.GetFirstPriority())
			return 0;
		return (currentClock - this->oneShotTimerQueue.GetFirstPriority()) / 1000;
	}

	return Natural<uint64>::Max();
}