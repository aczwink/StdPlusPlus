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
#include "CocoaRenderTargetWidgetBackend.hh"
//Local
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Objective-C class
@implementation OpenGLView
{
	_stdxx_::CocoaRenderTargetWidgetBackend *backend;
}

- (id)initWithBackend:(_stdxx_::CocoaRenderTargetWidgetBackend *)cocoaWindowBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = cocoaWindowBackend;
	return self;
}

//Methods
- (Math::PointD)GetClickLocation:(NSEvent *)event
{
	NSPoint p = [self convertPoint:[event locationInWindow] fromView:nil];
	return Math::PointD(p.x, p.y);
}

- (KeyboardModifiers)GetKeyboardModifiers:(NSEvent *)event
{
	KeyboardModifiers keyboardModifiers;

	keyboardModifiers.ctrl = [event modifierFlags] & NSEventModifierFlagControl;

	return keyboardModifiers;
}

//Override
- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];

	Event e(EventType::WidgetShouldBePainted);
	self->backend->GetWidget().Event(e);
}

- (void)mouseDragged:(NSEvent *)event
{
	NSPoint at = [self convertPoint:[event locationInWindow] fromView:nil];
	if(NSPointInRect(at, [self frame]))
	{
		MouseEvent mouseEvent(EventType::MouseMoved, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
		self->backend->GetWidget().Event(mouseEvent);
	}
}

- (void) mouseDown: (NSEvent*) event
{
	MouseClickEvent mouseClickEvent(MouseButton::Left, true, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
	self->backend->GetWidget().Event(mouseClickEvent);
}

- (void)mouseMoved:(NSEvent *)event
{
	NSPoint at = [self convertPoint:[event locationInWindow] fromView:nil];
	if(NSPointInRect(at, [self frame]))
	{
		MouseEvent mouseEvent(EventType::MouseMoved, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
		self->backend->GetWidget().Event(mouseEvent);
	}
}

- (void) mouseUp: (NSEvent*) event
{
	MouseClickEvent mouseClickEvent(MouseButton::Left, false, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
	self->backend->GetWidget().Event(mouseClickEvent);
}

- (void)rightMouseDown:(NSEvent *)event
{
	MouseClickEvent mouseClickEvent(MouseButton::Right, true, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
	self->backend->GetWidget().Event(mouseClickEvent);
}

- (void)rightMouseUp:(NSEvent *)event
{
	MouseClickEvent mouseClickEvent(MouseButton::Right, false, [self GetClickLocation:event], [self GetKeyboardModifiers:event]);
	self->backend->GetWidget().Event(mouseClickEvent);
}

- (void)scrollWheel:(NSEvent *)event
{
	MouseWheelEvent mouseWheelEvent([event scrollingDeltaY], [self GetKeyboardModifiers:event]);
	self->backend->GetWidget().Event(mouseWheelEvent);
}

@end

//Constructor
CocoaRenderTargetWidgetBackend::CocoaRenderTargetWidgetBackend(UIBackend& uiBackend, Widget *widget) : CocoaWidgetBackend(uiBackend), WidgetBackend(uiBackend), widget(widget)
{
	this->openGLView = [[OpenGLView alloc] initWithBackend:this];
}

//Destructor
CocoaRenderTargetWidgetBackend::~CocoaRenderTargetWidgetBackend()
{
	[this->openGLView release];
}

//Public methods
Math::SizeD CocoaRenderTargetWidgetBackend::GetSizeHint() const
{
	//intrinsicContentSize doesn't work for this one. It returns always Size(-1, -1)
	return StdXX::Math::SizeD(); //no idea...
}

NSView *CocoaRenderTargetWidgetBackend::GetView()
{
	return this->openGLView;
}

Widget &CocoaRenderTargetWidgetBackend::GetWidget()
{
	return *this->widget;
}

const Widget &CocoaRenderTargetWidgetBackend::GetWidget() const
{
	return *this->widget;
}

void CocoaRenderTargetWidgetBackend::Repaint()
{
	[this->openGLView setNeedsDisplay:YES];
}






//OLD STUFF:
void CocoaRenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}