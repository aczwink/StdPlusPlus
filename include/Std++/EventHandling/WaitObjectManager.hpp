/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Definitions.h>
#include <Std++/OSHandle.hpp>

namespace StdXX::EventHandling
{
	//Forward declarations
	class EventSource;

	struct WaitResult
	{
		OSHandle osHandle;
		int16 occuredEvents;
	};

	class WaitObjectManager
	{
	public:
		//Desructor
		virtual ~WaitObjectManager() = default;

		//Methods
		virtual void Add(const EventSource& source, int fd, int16 nativeEvents) = 0;
		virtual void AddWaitForInput(const EventSource& source, const OSHandle& osHandle) = 0;
		virtual void AddWaitForInput(const EventSource& source, int fd) = 0;
		virtual void Clear() = 0;
		virtual FixedArray<WaitResult> FetchWaitResult(const EventSource& eventSource) = 0;
	};
}