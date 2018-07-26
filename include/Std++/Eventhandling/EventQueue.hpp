/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/DynamicArray.hpp>
#include "Std++/Containers/PriorityQueue.hpp"
#include "EventSource.hpp"

namespace StdXX
{
	class STDPLUSPLUS_API EventQueue
	{
	public:
		//Constructor
		EventQueue();

		//Destructor
		~EventQueue();

		//Methods
		bool EventsPending();

		/**
		 * Dispatches pending events in this event queue and, if \p block is true, waits until new events arrive.
		 *
		 * @param block If \p block is true, the method waits (not busy) for new events. Else the function returns when all pending events were dispatched.
		 * @return false if this event queue was asked to quit and all pending events were dispatched, else true.
		 */
		bool ProcessEvents(bool block = true);

		//Inline
		inline void AddSource(EventSource *source)
		{
			this->sources.Push(source);
		}
		/**
		 * Indicates that this event queue should be shut down.
		 * All pending events will still be dispatched.
		 */
		inline void Quit()
		{
			this->quit = true;
		}

	private:
		//Members
		bool quit;
		DynamicArray<EventSource *> sources;
		void *internal;

		//Methods
		void DispatchPendingEvents();
		void System_CollectWaitObjects();
		void System_Init();
		void System_Shutdown();
		void System_WaitForEvents(uint64 timeOut);
		void WaitForEvents();
	};
}