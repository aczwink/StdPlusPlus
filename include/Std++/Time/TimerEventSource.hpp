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
#pragma once
//Local
#include <Std++/EventHandling/EventSource.hpp>
#include <Std++/Containers/PriorityQueue.hpp>
#include <Std++/Tuple.hpp>
#include "Clock.hpp"

namespace StdXX
{
	//Forward declarations
	class Timer;

	class TimerEventSource : public EventHandling::EventSource
	{
	public:
		//Class members
		static TimerEventSource *globalSource;

		//Methods
		bool CheckWaitResults(const FixedArray<EventHandling::WaitResult> &waitResults) override;
		void DispatchPendingEvents() override;
		bool HasPendingEvents() const override;
		uint64 QueryWaitInfo(EventHandling::WaitObjectManager &waitObjectManager) override;

		//Inline
		/**
		 *
		 * @param timeOut in nanoseconds
		 * @param timer
		 */
		inline void AddOneShotTimer(uint64 timeOut, Timer *timer)
		{
			this->oneShotTimerQueue.Insert({this->clock.GetCurrentValue() + timeOut, timer});
			this->eventTriggerer.Signal();
		}

		inline void RemoveTimer(Timer *timer)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
			//this->oneShotTimerQueue.Remove(timer);
		}

	private:
		//Members
		EventHandling::EventSignal eventTriggerer;
		Clock clock;
		PriorityQueue<Tuple<uint64, Timer *>> oneShotTimerQueue;
	};
}