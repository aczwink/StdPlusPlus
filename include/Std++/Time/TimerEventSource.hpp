/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/Eventhandling/EventSource.hpp>
#include <Std++/Containers/PriorityQueue.hpp>
#include "Clock.hpp"

namespace StdXX
{
	//Forward declarations
	class Timer;

	class TimerEventSource : public EventSource
	{
	public:
		//Class members
		static TimerEventSource *globalSource;

		//Methods
		void DispatchPendingEvents() override;
		uint64 GetMaxTimeout() const override;

		//Inline
		/**
		 *
		 * @param timeOut in nanoseconds
		 * @param timer
		 */
		inline void AddOneShotTimer(uint64 timeOut, Timer *timer)
		{
			this->oneShotTimerQueue.Insert(this->clock.GetCurrentValue() + timeOut, timer);
		}

		inline void RemoveTimer(Timer *timer)
		{
			this->oneShotTimerQueue.Remove(timer);
		}

		void VisitWaitObjects(const Function<void(_stdxx_::WaitObjHandle, bool)> &visitFunc) const override;

	private:
		//Members
		Clock clock;
		PriorityQueue<Timer *, uint64> oneShotTimerQueue;
	};
}