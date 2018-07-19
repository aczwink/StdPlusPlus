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
#include "WindowDelegate.hh"
//Local
#include "CocoaEventSource.hh"
#include "CocoaWindowBackend.hh"
#import <Std++/UI/Window.hpp>
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus::UI;

@implementation WindowDelegate
{
	_stdpp::CocoaWindowBackend *backend;
}

- (id)initWithBackend:(_stdpp::CocoaWindowBackend *)cocoaWindowBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = cocoaWindowBackend;
	return self;
}

- (void)windowDidResize:(NSNotification *)notification
{
	CocoaEventSource::EmitResizedEvent( (StdPlusPlus::UI::Window &)self->backend->GetWidget() );
}

- (void)windowWillClose:(NSNotification *)notification
{
	CocoaEventSource::EmitCloseEvent( (StdPlusPlus::UI::Window &)self->backend->GetWidget() );
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
	NSRect r = [sender frame];
	CocoaEventSource::EmitResizingEvent( (StdPlusPlus::UI::Window &)self->backend->GetWidget(), StdPlusPlus::Rect(r.origin.x, r.origin.y, frameSize.width, frameSize.height));

	return frameSize;
}
@end