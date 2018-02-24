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
//Class header
#include <Std++/UI/EventQueue.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Constructor
EventQueue::EventQueue()
{
	NOT_IMPLEMENTED_ERROR;
}

//Destructor
EventQueue::~EventQueue()
{
	NOT_IMPLEMENTED_ERROR;
}

//Private methods
void EventQueue::DispatchSystemEvents()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint64 EventQueue::QueryMonotonicClock()
{
	NOT_IMPLEMENTED_ERROR;
	return 0;
}

void EventQueue::WaitForEvents(uint64 minWaitTime_usec)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Public methods
/*
void EventQueue::PostQuitEvent(int32 exitCode)
{
	PostQuitMessage(exitCode);
}

bool EventQueue::ProcessEvents(bool block)
{
	MSG msg;

	if (block)
	{
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			//TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	else
	{
	}

	return false;
}

bool EventQueue::ProcessPendingEvents(int32 &refExitCode)
{
	MSG msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			refExitCode = (int32)msg.wParam;
			return false;
		}

		DispatchMessage(&msg);
	}

	return true;
}*/