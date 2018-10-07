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
#include "CocoaSpinBoxBackend.hh"
//Local
#include <Std++/UI/Events/ValueChangedEvent.hpp>
#import "CocoaEventSource.hh"
#include <Std++/UI/Controls/SpinBox.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Objective-C classes
@implementation IntegerValueFormatter

- (BOOL)isPartialStringValid:(NSString*)partialString newEditingString:(NSString**)newString errorDescription:(NSString**)error
{
	if([partialString length] == 0)
		return YES;

	NSScanner* scanner = [NSScanner scannerWithString:partialString];
	int32_t value;
	if(!([scanner scanInt:&value] && [scanner isAtEnd]))
	{
		[scanner release];
		NSBeep();
		return NO;
	}
	[scanner release];
	if(value < self->min)
	{
		NSBeep();
		return NO;
	}
	else if(value > self->max)
	{
		NSBeep();
		return NO;
	}

	return YES;
}

- (void)setRange:(int32_t)min max:(int32_t)max
{
	self->min = min;
	self->max = max;
}
@end

@implementation TextFieldDelegate
{
	_stdxx_::CocoaSpinBoxBackend *backend;
}

- (id)initWithBackend:(_stdxx_::CocoaSpinBoxBackend *)cocoaWindowBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = cocoaWindowBackend;
	return self;
}

- (void)controlTextDidChange:(NSNotification *)notification
{
	self->backend->OnTextChanged();
}
@end

@implementation CocoaStepper
{
	CocoaSpinBoxBackend *backend;
}

- (id)initWithBackend:(CocoaSpinBoxBackend *)spinBoxBackend
{
	self = [super init];
	if(!self)
		return nil;
	self->backend = spinBoxBackend;
	return self;
}

- (void)OnValueChanged:(NSStepper *)sender
{
	self->backend->OnStepperClicked();
}
@end

//Constructor
CocoaSpinBoxBackend::CocoaSpinBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::SpinBox *spinBox)
		: SpinBoxBackend(uiBackend), WidgetBackend(uiBackend), spinBox(spinBox)
{
	this->textField = [[NSTextField alloc] init];
	this->textFieldDelegate = [[TextFieldDelegate alloc] initWithBackend:this];
	[this->textField setDelegate:this->textFieldDelegate];
	this->integerValueFormatter = [[IntegerValueFormatter alloc] init];
	[this->textField setFormatter:this->integerValueFormatter];

	this->stepper = [[CocoaStepper alloc] initWithBackend:this];
	[this->stepper setTarget:this->stepper];
	[this->stepper setAction:@selector(OnValueChanged:)];
}

//Destructor
CocoaSpinBoxBackend::~CocoaSpinBoxBackend()
{
	[this->textField release];
	[this->textFieldDelegate release];
	[this->stepper release];
	[this->integerValueFormatter release];
}

//Public methods
StdXX::Math::SizeD CocoaSpinBoxBackend::GetSizeHint() const
{
	NSSize s = [this->stepper intrinsicContentSize];
	return StdXX::Math::SizeD([this->textField intrinsicContentSize].width + 1.2f * s.width, s.height);
}

StdXX::UI::Widget &CocoaSpinBoxBackend::GetWidget()
{
	return *this->spinBox;
}

const StdXX::UI::Widget &CocoaSpinBoxBackend::GetWidget() const
{
	return *this->spinBox;
}

void CocoaSpinBoxBackend::IgnoreEvent()
{
}

void CocoaSpinBoxBackend::SetBounds(const StdXX::Math::RectD &area)
{
	NSSize s = [this->stepper intrinsicContentSize];
	StdXX::Math::RectD textFieldRect, stepperRect;
	textFieldRect = area;
	textFieldRect.width() -= s.width * 1.5f;
	stepperRect = area;
	stepperRect.x() = textFieldRect.GetHorizontalEnd() + 0.5f * s.width;
	stepperRect.width() = s.width;
	[this->textField setFrame:NSMakeRect(textFieldRect.origin.x, textFieldRect.origin.y, textFieldRect.width(), textFieldRect.height())];
	[this->stepper setFrame:NSMakeRect(stepperRect.origin.x, stepperRect.origin.y, stepperRect.width(), stepperRect.height())];
}

void CocoaSpinBoxBackend::SetEnabled(bool enable)
{
	[this->textField setEnabled:enable];
	[this->stepper setEnabled:enable];
}

void CocoaSpinBoxBackend::SetHint(const StdXX::String &text)
{
	NSString* tmp;
	if(text.IsEmpty())
		tmp = [[NSString new] autorelease];
	else
		tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->textField setToolTip:tmp];
	[this->stepper setToolTip:tmp];
}

void CocoaSpinBoxBackend::SetRange(int32 min, int32 max)
{
	[this->integerValueFormatter setRange:min max:max];
}

void CocoaSpinBoxBackend::SetValue(int32 value)
{
	[this->textField setStringValue:[NSString stringWithFormat: @"%d", value]];
	[this->stepper takeIntValueFrom:this->textField];
}

void CocoaSpinBoxBackend::Show(bool visible)
{
	[this->textField setHidden:!visible];
	[this->stepper setHidden:!visible];
}

//Event handlers
void CocoaSpinBoxBackend::OnStepperClicked()
{
	[this->textField takeIntValueFrom:this->stepper];

	Variant value;
	value.i32 = [this->stepper intValue];

	ValueChangedEvent event(value);
	this->spinBox->Event(event);
}

void CocoaSpinBoxBackend::OnTextChanged()
{
	[this->stepper takeIntValueFrom:this->textField];

	Variant value;
	value.i32 = this->GetValue();

	ValueChangedEvent event(value);
	this->spinBox->Event(event);
}

















//old stuff
void CocoaSpinBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}






//Private methods
int32 CocoaSpinBoxBackend::GetValue() const
{
	NSScanner* scanner = [NSScanner scannerWithString:[this->textField stringValue]];
	int32 value;
	[scanner scanInt:&value];
	[scanner release];
	return value;
}