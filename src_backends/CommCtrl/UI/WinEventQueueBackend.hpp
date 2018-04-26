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
//Local
#include <Std++/_Backends/EventQueueBackend.hpp>
#include <Std++/UI/EventQueue.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
	class WinEventQueueBackend : public EventQueueBackend
	{
	public:
		//Constructor
		inline WinEventQueueBackend(EventQueue &eventQueue) : eventQueue(eventQueue)
		{
		}

		//Methods
		void DispatchPendingEvents() override;
		void PostQuitEvent() override;
		void WaitForEvents(uint64 minWaitTime_usec) override;

	private:
		//Members
		EventQueue &eventQueue;
	};
}