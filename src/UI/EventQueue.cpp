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
//Local
#include <ACStdLib/Time/Timer.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Globals
EventQueue *g_globalEventQueue = nullptr;

//Public methods
bool EventQueue::ProcessEvents(bool block)
{
	if(block)
	{
		while(!this->quit)
		{
			uint64 minWaitTime_usec = this->GetShortestTimerTimeOut();
			this->DispatchPendingEvents();
			this->WaitForEvents(minWaitTime_usec);
		}

		this->DispatchPendingEvents();
	}
	else
	{
		this->DispatchPendingEvents();
	}

	return !this->quit;
}

//Class Functions
EventQueue &EventQueue::GetGlobalQueue()
{
	if(!g_globalEventQueue)
		g_globalEventQueue = new EventQueue;

	return *g_globalEventQueue;
}

//Private methods
void EventQueue::DispatchPendingEvents()
{
	this->DispatchTimers();
	if(this == &EventQueue::GetGlobalQueue())
		this->DispatchSystemEvents();
}

void EventQueue::DispatchTimers()
{
	uint64 currentClock = this->QueryMonotonicClock();
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
		uint64 currentClock = this->QueryMonotonicClock();
		if(currentClock >= this->oneShotTimerQueue.GetFirstPriority())
			return 0;
		return (currentClock - this->oneShotTimerQueue.GetFirstPriority()) / 1000;
	}

	return Natural<uint64>::Max();
}