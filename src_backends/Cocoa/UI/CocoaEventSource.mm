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
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;
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
	//cocoa does not give us an fd for poll or select.
	//we query cocoa events once per millisecond
	return 1000 * 1000;
}

void CocoaEventSource::VisitWaitObjects(const StdXX::Function<void(_stdxx_::WaitObjHandle, bool)> &visitFunc) const
{
	//none...
}

//Class functions
void CocoaEventSource::EmitCloseEvent(StdXX::UI::Window &window)
{
	l_eventSource->DispatchCloseEvent(window);
}

void CocoaEventSource::EmitMouseMovedEvent(StdXX::UI::Widget &widget, NSPoint point)
{
	l_eventSource->DispatchMouseMovedEvent(widget, Math::PointD(point.x, point.y));
}

void CocoaEventSource::EmitMousePressedEvent(StdXX::UI::Widget &widget, const StdXX::UI::Events::MouseClickEvent &event)
{
	l_eventSource->DispatchMouseButtonPressed(widget, event);
}

void CocoaEventSource::EmitMouseReleasedEvent(StdXX::UI::Widget &widget, const StdXX::UI::Events::MouseClickEvent &event)
{
	l_eventSource->DispatchMouseButtonReleased(widget, event);
}

void CocoaEventSource::EmitMouseWheelEvent(StdXX::UI::Widget &widget, float64 delta)
{
	l_eventSource->DispatchMouseWheelEvent(widget, delta);
}

void CocoaEventSource::EmitPaintEvent(StdXX::UI::Widget &widget)
{
	l_eventSource->DispatchPaintEvent(widget);
}

void CocoaEventSource::EmitResizedEvent(StdXX::UI::Widget &widget)
{
	l_eventSource->DispatchResizedEvent(widget);
}

void CocoaEventSource::EmitResizingEvent(StdXX::UI::Widget &widget, const StdXX::Math::RectD &newBounds)
{
	l_eventSource->DispatchResizingEvent(widget, newBounds);
}