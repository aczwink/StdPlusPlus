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
#import <Std++/UI/Events/ValueChangedEvent.hpp>
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
CocoaCheckBoxBackend::CocoaCheckBoxBackend(UIBackend& uiBackend, CheckBox *checkBox) : CheckBoxBackend(uiBackend), CocoaWidgetBackend(uiBackend), WidgetBackend(uiBackend), checkBox(checkBox)
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
	Variant value;
	value.b = [this->cocoaCheckBox state] == NSControlStateValueOn;

	ValueChangedEvent event(value);
	this->checkBox->Event(event);
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

const Widget &CocoaCheckBoxBackend::GetWidget() const
{
	return *this->checkBox;
}

	//return [this->cocoaCheckBox state] == NSControlStateValueOn;

void CocoaCheckBoxBackend::SetEnabled(bool enable)
{
	[this->cocoaCheckBox setEnabled:enable];
}

void CocoaCheckBoxBackend::SetText(const String &text)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->cocoaCheckBox setTitle:tmp];
	[tmp release];
}

void CocoaCheckBoxBackend::UpdateCheckState()
{
	NSControlStateValue value = this->checkBox->IsChecked() ? NSControlStateValueOn : NSControlStateValueOff;
	[this->cocoaCheckBox setState:value];
}










//old stuff:
void CocoaCheckBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaCheckBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}