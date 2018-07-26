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
#import "CocoaView.hh"
//Local
#import <Std++/_Backends/UI/UIBackend.hpp>
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdxx_;

//Public methods
void CocoaView::IgnoreEvent()
{
}

void CocoaView::SetBounds(const StdXX::Math::RectD &area)
{
	CocoaEventSource::EmitResizingEvent(this->GetWidget(), area);
	[this->GetView() setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height())];
	CocoaEventSource::EmitResizedEvent(this->GetWidget());
}