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
#include <Std++/EventHandling/EventQueue.hpp>
//Local
#include "../../posix_shared/EventHandling/poll_WaitObjectManager.hpp"
//Namespaces
using namespace StdXX::EventHandling;
//Definitions
#define THIS (*(_stdxx_::poll_WaitObjectManager*)this->waitObjectManager.operator->())

//Private methods
void EventQueue::System_WaitForEvents(uint64 timeOut)
{
	timespec waitTime;

	waitTime.tv_sec = static_cast<__time_t>(timeOut / 1000000000);
	waitTime.tv_nsec = static_cast<__syscall_slong_t>(timeOut % 1000000000);

	int ret = ppoll(THIS.PollFDs(), THIS.PollFDsCount(), &waitTime, nullptr);
	ASSERT(ret >= 0, u8"TODO: IMPLEMENT THIS CORRECTLY");
}