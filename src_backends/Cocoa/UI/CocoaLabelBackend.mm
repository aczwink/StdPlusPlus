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
#include "CocoaLabelBackend.hh"
//Local
#include <Std++/UI/Controls/Label.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
CocoaLabelBackend::CocoaLabelBackend(UIBackend& uiBackend, Label *label) : LabelBackend(uiBackend), CocoaWidgetBackend(uiBackend), WidgetBackend(uiBackend), label(label)
{
	this->textField = [[NSTextField alloc] init];
	[this->textField setBezeled:NO];
	[this->textField setDrawsBackground:NO];
	[this->textField setEditable:NO];
}

//Destructor
CocoaLabelBackend::~CocoaLabelBackend()
{
	[this->textField release];
}

//Public methods
Math::SizeD CocoaLabelBackend::GetSizeHint() const
{
	NSSize s = [this->textField intrinsicContentSize];
	return StdXX::Math::SizeD(s.width, s.height);
}

Widget &CocoaLabelBackend::GetWidget()
{
	return *this->label;
}

const Widget &CocoaLabelBackend::GetWidget() const
{
	return *this->label;
}

NSView *CocoaLabelBackend::GetView()
{
	return this->textField;
}

void CocoaLabelBackend::SetEnabled(bool enable)
{
	[this->textField setEnabled:enable];
}

void CocoaLabelBackend::SetText(const StdXX::String &text)
{
	NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(text.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
	[this->textField setStringValue:tmp];
	[tmp release];
}













//OLD STUFF
void CocoaLabelBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaLabelBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}