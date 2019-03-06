/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>
#include <Std++/Unsigned.hpp>
#include <Std++/Time/Timer.hpp>
//Namespaces
using namespace StdXX;

//Class members
TimerEventSource *TimerEventSource::globalSource = nullptr;

//Public methods
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

uint64 TimerEventSource::GetMaxTimeout() const
{
	if(!this->oneShotTimerQueue.IsEmpty())
	{
		uint64 currentClock = this->clock.GetCurrentValue();
		if(currentClock >= this->oneShotTimerQueue.Top().Get<0>())
			return 0;
		return (currentClock - this->oneShotTimerQueue.Top().Get<0>()) / 1000;
	}

	return Unsigned<uint64>::Max();
}

void TimerEventSource::VisitWaitObjects(const Function<void(_stdxx_::WaitObjHandle, bool)> &visitFunc)
{
	visitFunc(this->eventTriggerer.GetNativeHandle(), true);
}