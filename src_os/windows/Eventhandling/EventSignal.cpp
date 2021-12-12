/*
 * Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Eventhandling/EventSignal.hpp>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX::EventHandling;

//Constructor
EventSignal::EventSignal()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Destructor
EventSignal::~EventSignal()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Public methods
void EventSignal::Clear()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void EventSignal::Signal()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

/*//Global
#include <Windows.h>

//Constructor
EventWaiter::EventWaiter() : owned(true)
{
	this->nativeHandle.handle = CreateEventA(nullptr, FALSE, FALSE, nullptr);
}

//Destructor
EventWaiter::~EventWaiter()
{
	if (this->owned)
		CloseHandle(this->nativeHandle.handle);
}

//Public methods
void EventWaiter::Signal()
{
	SetEvent(this->nativeHandle.handle);
}*/