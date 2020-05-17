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
bool TimerEventSource::CheckWaitResults(const FixedArray<EventHandling::WaitResult> &waitResults)
{
	return waitResults[0].occuredEvents != 0;
}

void TimerEventSource::DispatchPendingEvents()
{
	uint64 currentClock = this->clock.GetCurrentValue();
	while(!this->oneShotTimerQueue.IsEmpty())
	{
		if(currentClock >= this->oneShotTimerQueue.Top().Get<0>())
		{
			this->oneShotTimerQueue.PopTop().Get<1>()->timedOutCallback();
		}
		else
		{
			break;
		}
	}
}

bool TimerEventSource::HasPendingEvents() const
{
	if(!this->oneShotTimerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->oneShotTimerQueue.Top().Get<0>())
			return true;
	}
	return false;
}

uint64 TimerEventSource::QueryWaitInfo(EventHandling::WaitObjectManager& waitObjectManager)
{
	waitObjectManager.AddWaitForInput(*this, this->eventTriggerer.Handle());

	if(!this->oneShotTimerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->oneShotTimerQueue.Top().Get<0>())
			return 0;
		return (currentClock - this->oneShotTimerQueue.Top().Get<0>()) / 1000;
	}

	return Unsigned<uint64>::Max();
}