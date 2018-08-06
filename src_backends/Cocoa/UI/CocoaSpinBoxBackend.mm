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
#import "CocoaEventSource.hh"
#include <Std++/UI/Controls/SpinBox.hpp>
//Namespaces
using namespace _stdxx_;

//Objective-C class
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

//Constructor
CocoaSpinBoxBackend::CocoaSpinBoxBackend(StdXX::UIBackend *uiBackend, StdXX::UI::SpinBox *spinBox)
		: SpinBoxBackend(uiBackend), WidgetBackend(uiBackend), spinBox(spinBox)
{
	this->textField = [[NSTextField alloc] init];
	this->stepper = [[NSStepper alloc] init];
	this->integerValueFormatter = [[IntegerValueFormatter alloc] init];
	[this->textField setFormatter:this->integerValueFormatter];
}

//Destructor
CocoaSpinBoxBackend::~CocoaSpinBoxBackend()
{
	[this->textField release];
	[this->stepper release];
	[this->integerValueFormatter release];
}

//Public methods
StdXX::Math::SizeD CocoaSpinBoxBackend::GetSizeHint() const
{
	NSSize s = [this->stepper intrinsicContentSize];
	return StdXX::Math::SizeD([this->textField intrinsicContentSize].width + 1.2f * s.width, s.height);
}

int32 CocoaSpinBoxBackend::GetValue() const
{
	NSScanner* scanner = [NSScanner scannerWithString:[this->textField stringValue]];
	int32 value;
	[scanner scanInt:&value];
	[scanner release];
	return value;
}

void CocoaSpinBoxBackend::IgnoreEvent()
{
}

void CocoaSpinBoxBackend::SetBounds(const StdXX::Math::RectD &area)
{
	CocoaEventSource::EmitResizingEvent(*this->spinBox, area);

	NSSize s = [this->stepper intrinsicContentSize];
	StdXX::Math::RectD textFieldRect, stepperRect;
	textFieldRect = area;
	textFieldRect.width() -= s.width * 1.5f;
	stepperRect = area;
	stepperRect.x() = textFieldRect.GetHorizontalEnd() + 0.5f * s.width;
	stepperRect.width() = s.width;
	[this->textField setFrame:NSMakeRect(textFieldRect.origin.x, textFieldRect.origin.y, textFieldRect.width(), textFieldRect.height())];
	[this->stepper setFrame:NSMakeRect(stepperRect.origin.x, stepperRect.origin.y, stepperRect.width(), stepperRect.height())];

	CocoaEventSource::EmitResizedEvent(*this->spinBox);
}

void CocoaSpinBoxBackend::SetRange(int32 min, int32 max)
{
	int32 oldValue = this->GetValue();
	[this->integerValueFormatter setRange:min max:max];
	if(oldValue < min)
		this->SetValue(min);
	else if(oldValue > max)
		this->SetValue(max);
}

void CocoaSpinBoxBackend::SetValue(int32 value)
{
	[this->textField setStringValue:[NSString stringWithFormat: @"%d", value]];
}

















//old stuff
WidgetBackend *CocoaSpinBoxBackend::CreateChildBackend(StdXX::UI::Widget *widget) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

StdXX::Math::SizeD CocoaSpinBoxBackend::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void CocoaSpinBoxBackend::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 CocoaSpinBoxBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void CocoaSpinBoxBackend::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaSpinBoxBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}