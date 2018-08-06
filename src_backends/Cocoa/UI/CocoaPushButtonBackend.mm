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
#include "CocoaPushButtonBackend.hh"
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Objective-C class
@implementation CocoaPushButton
{
	CocoaPushButtonBackend *backend;
}

- (id)initWithBackend:(_stdxx_::CocoaPushButtonBackend *)pushButtonBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = pushButtonBackend;
	return self;
}

- (void)OnClick:(id)OnClick
{
	self->backend->Clicked();
}
@end

//Constructor
CocoaPushButtonBackend::CocoaPushButtonBackend(UIBackend *uiBackend, PushButton *pushButton) : PushButtonBackend(uiBackend), CocoaView(uiBackend), WidgetBackend(uiBackend), pushButton(pushButton)
{
	this->cocoaButton = [[CocoaPushButton alloc] initWithBackend:this];
	[this->cocoaButton setBezelStyle:NSRoundedBezelStyle];
	[this->cocoaButton setTarget:this->cocoaButton];
	[this->cocoaButton setAction:@selector(OnClick:)];
}

//Destructor
CocoaPushButtonBackend::~CocoaPushButtonBackend()
{
	[this->cocoaButton release];
}

//Public methods
void CocoaPushButtonBackend::Clicked()
{
	if(this->pushButton->onActivatedHandler.IsBound())
		this->pushButton->onActivatedHandler();
}

Math::SizeD CocoaPushButtonBackend::GetSizeHint() const
{
	NSSize s = [this->cocoaButton intrinsicContentSize];
	return StdXX::Math::SizeD(s.width, s.height);
}

NSView *CocoaPushButtonBackend::GetView()
{
	return this->cocoaButton;
}

Widget &CocoaPushButtonBackend::GetWidget()
{
	return *this->pushButton;
}

void CocoaPushButtonBackend::SetText(const StdXX::String &text)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->cocoaButton setTitle:tmp];
	[tmp release];
}










//OLD STUFF
WidgetBackend *CocoaPushButtonBackend::CreateChildBackend(StdXX::UI::Widget *widget) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

Math::SizeD CocoaPushButtonBackend::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void CocoaPushButtonBackend::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path CocoaPushButtonBackend::SelectExistingDirectory(const StdXX::String &title,
													 const StdXX::Function<bool(StdXX::Path &)> callback) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Path();
}

void CocoaPushButtonBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 CocoaPushButtonBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void CocoaPushButtonBackend::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaPushButtonBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}