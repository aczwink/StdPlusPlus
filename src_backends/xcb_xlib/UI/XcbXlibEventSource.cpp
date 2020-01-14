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
#include "XcbXlibEventSource.hpp"
//Local
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//X Layer
#include "../XLayer/XConnection.hpp"

//Public methods
void XcbXlibEventSource::DispatchPendingEvents()
{
	xcb_generic_event_t* event;
	while( (event = xcb_poll_for_event(this->xConnection.Connection())) != nullptr )
	{
		switch(event->response_type & ~0x80)
		{
		}

		free(event);
	}
}

uint64 XcbXlibEventSource::GetMaxTimeout() const
{
	return Unsigned<uint64>::Max();
}

void XcbXlibEventSource::VisitWaitObjects(const Function<void(WaitObjHandle, bool)> &visitFunc)
{
	WaitObjHandle waitObjHandle{};
	waitObjHandle.fd = xcb_get_file_descriptor(this->xConnection.Connection());

	visitFunc(waitObjHandle, true);
}
