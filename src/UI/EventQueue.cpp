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

//Public methods
bool EventQueue::ProcessEvents(bool block)
{
	if(block)
	{
		while(true) //TODO: quit event
		{
			uint64 minWaitTime_usec = this->UpdateTimers();
			this->DispatchPendingEvents();
			this->WaitForEvents(minWaitTime_usec);
		}
	}
	else
	{
		this->UpdateTimers();
		this->DispatchPendingEvents();
	}

	return true; //TODO: quit event
}

//Class Functions
EventQueue &EventQueue::GetGlobalQueue()
{
	static EventQueue eventQueue;

	return eventQueue;
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
	for(auto it = this->oneShotTimerQueue.begin(); it != this->oneShotTimerQueue.end(); )
	{
		if((*it).leftTime_usec <= 0)
		{
			(*it).timer->timedOutCallback();
			it.Remove();
		}
		else
		{
			++it;
		}
	}
}

uint64 EventQueue::UpdateTimers()
{
	uint64 minWaitTime_usec = Natural<uint64>::Max();

	for(TimerEntry &entry : this->oneShotTimerQueue)
	{
		entry.leftTime_usec -= this->clock.GetElapsedMicroseconds();
		minWaitTime_usec = MIN(minWaitTime_usec, (uint64)entry.leftTime_usec);
	}
	this->clock.Start(); //restart the clock

	return minWaitTime_usec;
}