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
#include "OpenGLView.hh"
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Constructor
CocoaWindowBackend::CocoaWindowBackend(UIBackend *uiBackend, WindowBackendType type, Widget *widget) : WindowBackend(uiBackend, type, widget)
{
	switch(type)
	{
		case WindowBackendType::CheckBox:
		{
			this->button = [[NSButton alloc] init];
			[this->button setButtonType:NSSwitchButton];
		}
		break;
		case WindowBackendType::GroupBox:
		{
			this->groupBox = [[NSBox alloc] init];
		}
		break;
		case WindowBackendType::Label:
		{
			this->textField = [[NSTextField alloc] init];
			[this->textField setBezeled:NO];
			[this->textField setDrawsBackground:NO];
			[this->textField setEditable:NO];
		}
		break;
		case WindowBackendType::PushButton:
		{
			this->button = [[NSButton alloc] init];
			[this->button setBezelStyle:NSRoundedBezelStyle];
		}
		break;
		case WindowBackendType::RenderTarget:
		{
			this->openGLView = [[OpenGLView alloc] initWithBackend:this];
		}
		break;
		case WindowBackendType::Slider:
		{
			this->slider = [[NSSlider alloc] init];
		}
		break;
		case WindowBackendType::Window:
		{
			NSWindowStyleMask windowStyleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskUnifiedTitleAndToolbar;
			this->window = [[NSWindow alloc]
					initWithContentRect:NSMakeRect(0, 0, 1, 1)
							  styleMask:windowStyleMask
								backing:NSBackingStoreBuffered
								  defer:NO];
			this->windowDelegate = [[WindowDelegate alloc] initWithBackend:this];
			[this->window setDelegate:this->windowDelegate];
			this->windowController = [[NSWindowController alloc] initWithWindow:this->window];
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

//Destructor
CocoaWindowBackend::~CocoaWindowBackend()
{
	switch(this->type)
	{
		case WindowBackendType::CheckBox:
		case WindowBackendType::PushButton:
		{
			[this->button release];
		}
		break;
		case WindowBackendType::GroupBox:
		{
			[this->groupBox release];
		}
		break;
		case WindowBackendType::Label:
		{
			[this->textField release];
		}
		break;
		case WindowBackendType::RenderTarget:
		{
			[this->openGLView release];
		}
		break;
		case WindowBackendType::Slider:
		{
			[this->slider release];
		}
		break;
		case WindowBackendType::Window:
		{
			[this->windowDelegate release];
			[this->windowController release];
			[this->window release];
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

//Public methods
WindowBackend *CocoaWindowBackend::CreateChildBackend(WindowBackendType type, Widget *widget) const
{
	CocoaWindowBackend *child = new CocoaWindowBackend(this->GetUIBackend(), type, widget);

	[this->GetChildrenAreaView() addSubview:child->GetView()];

	return child;
}

StdPlusPlus::Rect _stdpp::CocoaWindowBackend::GetChildrenRect() const
{
	switch(this->type)
	{
		case WindowBackendType::GroupBox:
		{
			NSRect frame = [this->GetChildrenAreaView() frame];
			return StdPlusPlus::Rect(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
		}
		case WindowBackendType::Window:
		{
			NSRect frame = [[this->window contentView] frame];
			return StdPlusPlus::Rect(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
		}
	}
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdPlusPlus::Rect();
}

StdPlusPlus::Size _stdpp::CocoaWindowBackend::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdPlusPlus::Size();
}

StdPlusPlus::Size _stdpp::CocoaWindowBackend::GetSizeHint() const
{
	switch(this->type)
	{
		//at least the title must be viewable
		case WindowBackendType::CheckBox:
		{
			NSButtonCell *buttonCell = [[NSButtonCell alloc] init];
			auto s = this->ComputeTextSize([this->button title], [buttonCell font]);
			[buttonCell release];

			return s;
		}
		case WindowBackendType::GroupBox:
			return this->ComputeTextSize([this->groupBox title], [this->groupBox titleFont]);
		case WindowBackendType::Label:
			return this->ComputeTextSize([this->textField stringValue], [this->textField font]);
		case WindowBackendType::PushButton:
		{
			NSButtonCell *buttonCell = [[NSButtonCell alloc] init];
			auto s = this->ComputeTextSize([this->button title], [buttonCell font]);
			[buttonCell release];

			return s;
		}
		case WindowBackendType::RenderTarget:
			return StdPlusPlus::Size(); //no idea...
		case WindowBackendType::Slider:
		{
			NSSize s = [this->button intrinsicContentSize];
			return StdPlusPlus::Size(Math::Abs(s.width), s.height);
		}
		case WindowBackendType::Window:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdPlusPlus::Size();
}

void _stdpp::CocoaWindowBackend::Maximize()
{
	//[this->window setFrame:[[NSScreen mainScreen] visibleFrame] display:true animate:true];
	[this->window zoom:this->window];
}

void _stdpp::CocoaWindowBackend::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::Repaint()
{
	[this->GetView() setNeedsDisplay:YES];
}

void _stdpp::CocoaWindowBackend::Select(StdPlusPlus::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

StdPlusPlus::Path _stdpp::CocoaWindowBackend::SelectExistingDirectory(const StdPlusPlus::String &title,
																	  const StdPlusPlus::Function<bool(
																			  StdPlusPlus::Path &)> callback) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdPlusPlus::Path();
}

void _stdpp::CocoaWindowBackend::SetBounds(const StdPlusPlus::Rect &area)
{
	if(this->type == WindowBackendType::Window)
	{
		[this->window setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height()) display:YES];
	}
	else
	{
		CocoaEventSource::EmitResizingEvent(*this->widget, area);
		[this->GetView() setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height())];
		CocoaEventSource::EmitResizedEvent(*this->widget);
	}
}

void _stdpp::CocoaWindowBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::SetHint(const StdPlusPlus::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::SetMaximum(uint32 max)
{
	[this->slider setMaxValue:max];
}

void _stdpp::CocoaWindowBackend::SetPosition(uint32 pos) const
{
	[this->slider setDoubleValue:pos];
}

void _stdpp::CocoaWindowBackend::SetText(const StdPlusPlus::String &text)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	switch(this->type)
	{
		case WindowBackendType::GroupBox:
			[this->groupBox setTitle:tmp];
			break;
		case WindowBackendType::Label:
			[this->textField setStringValue:tmp];
			break;
		case WindowBackendType::CheckBox:
		case WindowBackendType::PushButton:
			[this->button setTitle:tmp];
			break;
		case WindowBackendType::Window:
			[this->window setTitle:tmp];
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	[tmp release];
}

void _stdpp::CocoaWindowBackend::Show(bool visible)
{
	switch(this->type)
	{
		case WindowBackendType::Window:
			[this->windowController showWindow:this->windowController];
			[this->window orderFrontRegardless];
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

void _stdpp::CocoaWindowBackend::ShowInformationBox(const StdPlusPlus::String &title,
													const StdPlusPlus::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::UpdateSelection(StdPlusPlus::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

bool _stdpp::CocoaWindowBackend::IsChecked() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

void _stdpp::CocoaWindowBackend::IgnoreEvent()
{
}

uint32 _stdpp::CocoaWindowBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void _stdpp::CocoaWindowBackend::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

int32 _stdpp::CocoaWindowBackend::GetValue() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

NSView *CocoaWindowBackend::GetView() const
{
	switch(this->type)
	{
		case WindowBackendType::GroupBox:
			return this->groupBox;
		case WindowBackendType::Label:
			return this->textField;
		case WindowBackendType::CheckBox:
		case WindowBackendType::PushButton:
			return this->button;
		case WindowBackendType::RenderTarget:
			return this->openGLView;
		case WindowBackendType::Slider:
			return this->slider;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	return nil;
}

void _stdpp::CocoaWindowBackend::SetMenuBar(StdPlusPlus::UI::MenuBar *menuBar, _stdpp::MenuBarBackend *menuBarBackend)
{
}

void CocoaWindowBackend::SetMinimum(uint32 min)
{
	[this->slider setMinValue:min];
}

void _stdpp::CocoaWindowBackend::SetRange(int32 min, int32 max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::SetValue(int32 value)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdpp::CocoaWindowBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
NSView *CocoaWindowBackend::GetChildrenAreaView() const
{
	switch(this->type)
	{
		case WindowBackendType::GroupBox:
			return [this->groupBox contentView];
		case WindowBackendType::Window:
			return [this->window contentView];
	}

	return nil;
}

StdPlusPlus::Size CocoaWindowBackend::ComputeTextSize(NSString *string, NSFont *font) const
{
	NSDictionary *attributes = @{NSFontAttributeName: font};
	NSSize s = [string sizeWithAttributes:attributes];

	[attributes release];

	return StdPlusPlus::Size(s.width, s.height);
}
