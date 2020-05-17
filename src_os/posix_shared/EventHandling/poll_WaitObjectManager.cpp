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
//Class header
#include <Std++/Containers/Array/FixedArray.hpp>
#include "poll_WaitObjectManager.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::EventHandling;

//Public methods
void poll_WaitObjectManager::Add(const EventSource& source, int fd, int16 nativeEvents)
{
	uint32 startIndex = this->pollfds.GetNumberOfElements();

	pollfd pfd;
	pfd.fd = fd;
	pfd.events = nativeEvents;
	pfd.revents = 0;

	this->pollfds.Push(pfd);

	if(this->ranges.Contains(&source))
		startIndex = this->ranges.Get(&source).start;
	this->ranges.Insert(&source, { startIndex, this->pollfds.GetNumberOfElements() });
}

void poll_WaitObjectManager::AddWaitForInput(const EventSource& source, const OSHandle &osHandle)
{
	this->AddWaitForInput(source, osHandle.fd);
}

void poll_WaitObjectManager::AddWaitForInput(const EventSource& source, int fd)
{
	this->Add(source, fd, POLLIN);
}

void poll_WaitObjectManager::Clear()
{
	this->pollfds.Resize(0);
	this->ranges.Release();
}

FixedArray<WaitResult> poll_WaitObjectManager::FetchWaitResult(const EventSource &eventSource)
{
	const StdXX::Math::Range<uint32>& range = this->ranges.Get(&eventSource);
	FixedArray<WaitResult> result(range.end - range.start);
	for(uint32 i = 0; i < result.GetNumberOfElements(); i++)
		result[i] = { {this->pollfds[i].fd}, this->pollfds[i].revents };
	return result;
}
