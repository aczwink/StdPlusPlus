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

//Override
- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];

	CocoaEventSource::EmitPaintEvent(self->backend->GetWidget());
}

- (void)mouseDragged:(NSEvent *)event
{
	NSPoint at = [self convertPoint:[event locationInWindow] fromView:nil];
	if(NSPointInRect(at, [self frame]))
	{
		CocoaEventSource::EmitMouseMovedEvent(self->backend->GetWidget(), at);
	}
}

- (void) mouseDown: (NSEvent*) event
{
	CocoaEventSource::EmitMousePressedEvent(self->backend->GetWidget(), MouseButton::Left, [self convertPoint:[event locationInWindow] fromView:nil]);
}

- (void)mouseMoved:(NSEvent *)event
{
	NSPoint at = [self convertPoint:[event locationInWindow] fromView:nil];
	if(NSPointInRect(at, [self frame]))
	{
		CocoaEventSource::EmitMouseMovedEvent(self->backend->GetWidget(), at);
	}
}

- (void) mouseUp: (NSEvent*) event
{
	CocoaEventSource::EmitMouseReleasedEvent(self->backend->GetWidget(), MouseButton::Left, [self convertPoint:[event locationInWindow] fromView:nil]);
}

- (void)scrollWheel:(NSEvent *)event
{
	CocoaEventSource::EmitMouseWheelEvent(self->backend->GetWidget(), [event scrollingDeltaY]);
}

@end

//Constructor
CocoaRenderTargetWidgetBackend::CocoaRenderTargetWidgetBackend(UIBackend *uiBackend, Widget *widget) : CocoaView(uiBackend), WidgetBackend(uiBackend), widget(widget)
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


void CocoaRenderTargetWidgetBackend::Repaint()
{
	[this->openGLView setNeedsDisplay:YES];
}






//OLD STUFF:
WidgetBackend *
CocoaRenderTargetWidgetBackend::CreateChildBackend(StdXX::UI::Widget *widget) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

Math::SizeD CocoaRenderTargetWidgetBackend::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void CocoaRenderTargetWidgetBackend::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::IgnoreEvent()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 CocoaRenderTargetWidgetBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void CocoaRenderTargetWidgetBackend::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaRenderTargetWidgetBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}