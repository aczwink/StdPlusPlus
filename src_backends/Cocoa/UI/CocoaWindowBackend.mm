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
#include "CocoaWindowBackend.hh"
//Local
#import "CocoaEventSource.hh"
#import "CocoaCompositeView.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Objective-C class
@implementation WindowDelegate
{
	_stdxx_::CocoaWindowBackend *backend;
}

- (id)initWithBackend:(_stdxx_::CocoaWindowBackend *)cocoaWindowBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = cocoaWindowBackend;
	return self;
}

- (void)windowDidResize:(NSNotification *)notification
{
	NSWindow *window = [notification object];
	NSRect r = [window frame];
	self->backend->GetWidget().Resize(StdXX::Math::SizeD(r.size.width, r.size.height));
	//CocoaEventSource::EmitResizedEvent( self->backend->GetWindow() );
}

- (void)windowWillClose:(NSNotification *)notification
{
	CocoaEventSource::EmitCloseEvent( self->backend->GetWindow() );
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
	//NSRect r = [sender frame];
	//CocoaEventSource::EmitResizingEvent( self->backend->GetWindow(), StdXX::Math::RectD(r.origin.x, r.origin.y, frameSize.width, frameSize.height));

	return frameSize;
}
@end

//Constructor
CocoaWindowBackend::CocoaWindowBackend(UIBackend *uiBackend, Window *window) : WindowBackend(uiBackend), WidgetContainerBackend(uiBackend), WidgetBackend(uiBackend), window(window)
{
	NSWindowStyleMask windowStyleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskUnifiedTitleAndToolbar;
	this->cocoaWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 1, 1) styleMask:windowStyleMask
			backing:NSBackingStoreBuffered defer:NO];
	this->windowDelegate = [[WindowDelegate alloc] initWithBackend:this];
	[this->cocoaWindow setDelegate:this->windowDelegate];
	this->windowController = [[NSWindowController alloc] initWithWindow:this->cocoaWindow];
	[this->cocoaWindow setAcceptsMouseMovedEvents:YES];
}

//Destructor
CocoaWindowBackend::~CocoaWindowBackend()
{
	[this->windowDelegate release];
	[this->windowController release];
	[this->cocoaWindow release];
}

//Public methods
void CocoaWindowBackend::AddChild(StdXX::UI::Widget *widget)
{
	CocoaView *cocoaView = dynamic_cast<CocoaView *>(widget->_GetBackend());
	[this->cocoaWindow setContentView:cocoaView->GetView()];
}

CompositeWidget *CocoaWindowBackend::CreateContentArea()
{
	return new CocoaCompositeView(this->GetUIBackend(), [this->cocoaWindow contentView]);
}

Math::RectD CocoaWindowBackend::GetContentAreaBounds() const
{
	const NSRect f = [[this->cocoaWindow contentView] frame];
	return StdXX::Math::RectD(f.origin.x, f.origin.y, f.size.width, f.size.height);
}

Widget &CocoaWindowBackend::GetWidget()
{
	return *this->window;
}

const Widget &CocoaWindowBackend::GetWidget() const
{
	return *this->window;
}

void CocoaWindowBackend::IgnoreEvent()
{
}

void CocoaWindowBackend::Maximize()
{
	//[this->window setFrame:[[NSScreen mainScreen] visibleFrame] display:true animate:true];
	[this->cocoaWindow zoom:this->cocoaWindow];
}

Path CocoaWindowBackend::SelectExistingDirectory(const String &title, const Function<bool(Path &)> callback) const
{
	NSOpenPanel *panel = [[NSOpenPanel alloc] init];
	[panel setCanChooseFiles:false];
	[panel setCanChooseDirectories:true];
	[panel setAllowsMultipleSelection:false];

	String resultStr;
	if([panel runModal] == NSModalResponseOK)
	{
		NSURL *choice = [panel URLs][0];

		NSString *tmp = [NSString stringWithCString:u8"file" encoding:NSUTF8StringEncoding];
		ASSERT([[choice scheme] isEqualToString:tmp], u8"URLs are currently not supported...");
		[tmp release];

		NSString *str = [choice absoluteString];
		NSString *tmp2 = [str substringFromIndex:7];

		const char *result = [tmp2 cStringUsingEncoding:NSUTF8StringEncoding];
		resultStr = String::CopyRawString(result);
		[tmp2 release];
	}
	[panel release];

	return resultStr;
}

void CocoaWindowBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
	//TODO: implement me
}

void CocoaWindowBackend::SetTitle(const StdXX::String &title)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(title.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->cocoaWindow setTitle:tmp];
}

void CocoaWindowBackend::Show(bool visible)
{
	[this->windowController showWindow:this->windowController];
	[this->cocoaWindow orderFrontRegardless];
}








//OLD STUFF:
uint32 CocoaWindowBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

StdXX::Math::SizeD CocoaWindowBackend::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

NSView *CocoaWindowBackend::GetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nil;
}

void CocoaWindowBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::SetBounds(const StdXX::Math::RectD &area)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaWindowBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

StdXX::Math::SizeD CocoaWindowBackend::ComputeTextSize(NSString *string, NSFont *font) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

NSView *CocoaWindowBackend::GetChildrenAreaView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nil;
}


/*
void _stdxx_::CocoaWindowBackend::SetBounds(const StdXX::Math::RectD &area)
{
	if(this->type == WindowBackendType::Window)
	{
		[this->window setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height()) display:YES];
	}
}
*/