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
#include "CocoaEventSource.hh"
//Namespaces
using namespace _stdpp;
//Local variables
static CocoaEventSource *l_eventSource = nullptr;

//Constructor
CocoaEventSource::CocoaEventSource()
{
	l_eventSource = this;
}

//Public methods
void CocoaEventSource::DispatchPendingEvents()
{
	while(true)
	{
		NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
											untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
		if(event == nil)
			break;

		[NSApp sendEvent: event];
	}
}

uint64 CocoaEventSource::GetMaxTimeout() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void CocoaEventSource::VisitWaitObjects(const StdPlusPlus::Function<void(_stdpp::WaitObjHandle, bool)> &visitFunc) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Class functions
void CocoaEventSource::EmitCloseEvent(StdPlusPlus::UI::Window &window)
{
	l_eventSource->DispatchCloseEvent(window);
}
