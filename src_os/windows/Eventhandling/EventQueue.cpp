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
#include <Std++/Eventhandling/EventQueue.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace StdXX;
using namespace StdXX::EventHandling;
//Definitions
#define THIS ((DynamicArray<HANDLE> *)this->internal)

//Private methods
void EventQueue::System_Init()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void EventQueue::System_WaitForEvents(uint64 timeOut)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

/*//Private methods
void EventQueue::System_CollectWaitObjects()
{
	THIS->Resize(0);

	auto collector = [this](_stdxx_::WaitObjHandle waitObjHandle, bool input)
	{
		THIS->Push(waitObjHandle.handle);
	};

	for (EventSource *const& source : this->sources)
	{
		source->VisitWaitObjects(collector);
	}
}

void EventQueue::System_Init()
{
	this->internal = new DynamicArray<HANDLE>;
}

void EventQueue::System_Shutdown()
{
	delete THIS;
}

void EventQueue::System_WaitForEvents(uint64 timeOut)
{
	DWORD waitTime = static_cast<DWORD>(timeOut / 1000000);
	if(waitTime == 0)
		waitTime = 1; //'timeOut' can't be 0
	HANDLE *handles = nullptr;
	if (!THIS->IsEmpty())
		handles = &(*THIS)[0];
	MsgWaitForMultipleObjectsEx(THIS->GetNumberOfElements(), handles, waitTime, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
}*/