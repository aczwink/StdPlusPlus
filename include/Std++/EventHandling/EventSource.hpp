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
#include <Std++/Definitions.h>
#include <Std++/Function.hpp>
#include "EventSignal.hpp"
#include "WaitObjectManager.hpp"

namespace StdXX::EventHandling
{
	class EventSource
	{
	public:
		//Destructor
		virtual ~EventSource() = default;

		//Abstract
		virtual bool CheckWaitResults(const FixedArray<WaitResult>& waitResults) = 0;
		virtual void DispatchPendingEvents() = 0;
		virtual bool HasPendingEvents() const = 0;
		/**
		 * Queries the wait records that the queue will wait on for new events and the maximum time in nanoseconds until this source expects a new event.
		 * The source should write its wait records into \p waitObjectManager.
		 * Event queues will take this information to determine its sleeping duration.
		 *
		 * @return Timeout
		 */
		virtual uint64 QueryWaitInfo(WaitObjectManager& waitObjectManager) = 0;
	};
}