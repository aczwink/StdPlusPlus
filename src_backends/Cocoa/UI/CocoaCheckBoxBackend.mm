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
#include "CocoaCheckBoxBackend.hh"
//Local
#import <Std++/UI/Controls/CheckBox.hpp>
#import "CocoaEventSource.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Objective-C class
@implementation CocoaCheckBox
{
	CocoaCheckBoxBackend *backend;
}

- (id)initWithBackend:(_stdxx_::CocoaCheckBoxBackend *)checkBoxBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = checkBoxBackend;
	return self;
}

- (void)OnClick:(id)OnClick
{
	self->backend->Clicked();
}
@end

//Constructor
CocoaCheckBoxBackend::CocoaCheckBoxBackend(UIBackend *uiBackend, CheckBox *checkBox) : CheckBoxBackend(uiBackend, checkBox), CocoaView(uiBackend), WidgetBackend(uiBackend), checkBox(checkBox)
{
	this->cocoaCheckBox = [[CocoaCheckBox alloc] initWithBackend:this];
	[this->cocoaCheckBox setButtonType:NSSwitchButton];
	[this->cocoaCheckBox setTarget:this->cocoaCheckBox];
	[this->cocoaCheckBox setAction:@selector(OnClick:)];
}

//Destructor
CocoaCheckBoxBackend::~CocoaCheckBoxBackend()
{
	[this->cocoaCheckBox release];
}

//Public methods
void CocoaCheckBoxBackend::Clicked()
{
	if(this->checkBox->onToggledHandler.IsBound())
		this->checkBox->onToggledHandler();
}

SizeD CocoaCheckBoxBackend::GetSizeHint() const
{
	NSSize s = [this->cocoaCheckBox intrinsicContentSize];
	return SizeD(s.width, s.height);
}

NSView *CocoaCheckBoxBackend::GetView()
{
	return this->cocoaCheckBox;
}

Widget &CocoaCheckBoxBackend::GetWidget()
{
	return *this->checkBox;
}

bool CocoaCheckBoxBackend::IsChecked() const
{
	return [this->cocoaCheckBox state] == NSControlStateValueOn;
}

void CocoaCheckBoxBackend::SetText(const String &text)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->cocoaCheckBox setTitle:tmp];
	[tmp release];
}










//old stuff:
WidgetBackend *
CocoaCheckBoxBackend::CreateChildBackend(StdXX::UI::Widget *widget) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

StdXX::Math::SizeD CocoaCheckBoxBackend::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void CocoaCheckBoxBackend::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Path CocoaCheckBoxBackend::SelectExistingDirectory(const StdXX::String &title,
												   const StdXX::Function<bool(StdXX::Path &)> callback) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Path();
}

void CocoaCheckBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 CocoaCheckBoxBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void CocoaCheckBoxBackend::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}