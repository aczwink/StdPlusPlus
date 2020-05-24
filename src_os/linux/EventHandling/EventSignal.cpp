/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/EventHandling/EventSignal.hpp>
//Global
#include <sys/eventfd.h>
#include <unistd.h>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX::EventHandling;

//Constructor
EventSignal::EventSignal()
{
    this->waitableHandle.fd = eventfd(0, 0);
}

//Destructor
EventSignal::~EventSignal()
{
    close(this->waitableHandle.fd);
}

//Public methods
void EventSignal::Signal()
{
	const uint64 increment = 1;
	ssize_t ret = write(this->waitableHandle.fd, &increment, sizeof(increment));
	ASSERT(ret != -1, u8"TODO: IMPPLEMENT THIS CORRECTLY");
}