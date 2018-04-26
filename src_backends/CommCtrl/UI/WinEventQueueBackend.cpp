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
//Class header
#include "WinEventQueueBackend.hpp"
//Global
#include <Windows.h>
//Namespaces
using namespace _stdpp;

//Public methods
void WinEventQueueBackend::DispatchPendingEvents()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
			break;

		DispatchMessage(&msg);
	}
}

void WinEventQueueBackend::PostQuitEvent()
{
	PostQuitMessage(EXIT_SUCCESS);
}

void WinEventQueueBackend::WaitForEvents(uint64 minWaitTime_usec)
{
	MsgWaitForMultipleObjectsEx(0, nullptr, static_cast<DWORD>(minWaitTime_usec / 1000), QS_ALLINPUT, MWMO_INPUTAVAILABLE);
}
