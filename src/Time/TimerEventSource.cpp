/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Time/TimerEventSource.hpp>
//Local
#include <Std++/EventHandling/WaitObjectManager.hpp>
#include <Std++/Unsigned.hpp>
#include <Std++/Time/Timer.hpp>
//Namespaces
using namespace StdXX;

//Class members
TimerEventSource *TimerEventSource::globalSource = nullptr;

//Public methods
void TimerEventSource::AddTimerToQueue(Timer &timer)
{
	this->timerQueue.Insert({this->clock.GetCurrentValue() + timer.timeOut, &timer});
	this->eventTriggerer.Signal();
}

bool TimerEventSource::CheckWaitResults(const EventHandling::WaitResult &waitResults)
{
	if(waitResults.AnyEventOccured(this->eventTriggerer.Handle()))
	{
		this->eventTriggerer.Clear();
		return true;
	}
	return false;
}

void TimerEventSource::DispatchPendingEvents()
{
	uint64 currentClock = this->clock.GetCurrentValue();
	while(!this->timerQueue.IsEmpty())
	{
		if(currentClock >= this->timerQueue.Top().Get<0>())
		{
			Tuple<uint64, Timer*> top = this->timerQueue.PopTop();
			Timer* timer = top.Get<1>();

			bool wasPending = timer->isPending;

			if(!timer->IsPeriodic())
				timer->isPending = false;

			if(wasPending)
				timer->timedOutCallback();
			if(timer->IsPeriodic())
				this->AddTimerToQueue(*timer);
		}
		else
		{
			break;
		}
	}
}

bool TimerEventSource::HasPendingEvents() const
{
	if(!this->timerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->timerQueue.Top().Get<0>())
			return true;
	}
	return false;
}

uint64 TimerEventSource::QueryWaitInfo(EventHandling::WaitObjectManager& waitObjectManager)
{
	waitObjectManager.AddWaitForInput(*this, this->eventTriggerer.Handle());

	if(!this->timerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->timerQueue.Top().Get<0>())
			return 0;
		return (currentClock - this->timerQueue.Top().Get<0>());
	}

	return Unsigned<uint64>::Max();
}