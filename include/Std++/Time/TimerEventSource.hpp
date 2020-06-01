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
#include <Std++/Containers/Map/Map.hpp>
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
		void AddTimerToQueue(Timer& timer);
		bool CheckWaitResults(const EventHandling::WaitResult& waitResults) override;
		void DispatchPendingEvents() override;
		bool HasPendingEvents() const override;
		uint64 QueryWaitInfo(EventHandling::WaitObjectManager &waitObjectManager) override;

	private:
		//Members
		EventHandling::EventSignal eventTriggerer;
		Clock clock;
		PriorityQueue<Tuple<uint64, Timer *>, GreaterThan<Tuple<uint64, Timer *>>> timerQueue;
	};
}