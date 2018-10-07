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
#include "CocoaGroupBoxBackend.hh"
//Local
#include <Std++/UI/Containers/GroupBox.hpp>
#import "CocoaContainerBackend.hh"
#import "CocoaEventSource.hh"
#import "CocoaWidgetBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
CocoaGroupBoxBackend::CocoaGroupBoxBackend(UIBackend& uiBackend, GroupBox *groupBox) : CocoaWidgetBackend(uiBackend), GroupBoxBackend(uiBackend), WidgetContainerBackend(uiBackend), WidgetBackend(uiBackend), groupBox(groupBox)
{
	this->cocoaGroupBox = [[NSBox alloc] init];
	[this->cocoaGroupBox setTitle:[NSString new]];
}

//Destructor
CocoaGroupBoxBackend::~CocoaGroupBoxBackend()
{
	[this->cocoaGroupBox release];
}

//Public methods
void CocoaGroupBoxBackend::AddChild(StdXX::UI::Widget *widget)
{
	CocoaWidgetBackend *cocoaView = dynamic_cast<CocoaWidgetBackend *>(widget->_GetBackend());
	[this->cocoaGroupBox setContentView:cocoaView->GetView()];
}

WidgetContainerBackend *CocoaGroupBoxBackend::CreateContentAreaBackend(StdXX::UI::CompositeWidget &widget)
{
	return new CocoaContainerBackend(this->GetUIBackend(), widget, [this->cocoaGroupBox contentView]);
}

Math::RectD CocoaGroupBoxBackend::GetContentAreaBounds() const
{
	const NSRect f = [[this->cocoaGroupBox contentView] frame];
	return StdXX::Math::RectD(f.origin.x, f.origin.y, f.size.width, f.size.height);
}

Math::SizeD CocoaGroupBoxBackend::GetSizeHint() const
{
	Math::SizeD result;

	//don't call intrinsicContentSize, because this will somehow include the children
	/*
	//get min size
	NSSize minSize = [this->cocoaGroupBox intrinsicContentSize];
	result.width = minSize.width;
	result.height = minSize.height;
	 */

	//add title
	auto titleSize = this->ComputeTextSize([this->cocoaGroupBox title], [this->cocoaGroupBox titleFont]);
	//result.width = Math::Max(result.width, titleSize.width);
	result.height += titleSize.height;

	//add margin
	auto contentViewMargins = [this->cocoaGroupBox contentViewMargins];
	result.width += 2*contentViewMargins.width; //left and right
	result.height += 2*contentViewMargins.height; //top and bottom

	//add border
	auto borderWidth = [this->cocoaGroupBox borderWidth];
	result.width += 2* borderWidth; //left and right
	result.height += 2* borderWidth; //top and bottom

	return result;
}

NSView *CocoaGroupBoxBackend::GetView()
{
	return this->cocoaGroupBox;
}

Widget &CocoaGroupBoxBackend::GetWidget()
{
	return *this->groupBox;
}

const Widget &CocoaGroupBoxBackend::GetWidget() const
{
	return *this->groupBox;
}

void CocoaGroupBoxBackend::SetTitle(const StdXX::String &title)
{
	if(title.IsEmpty())
	{
		[this->cocoaGroupBox setTitle:[NSString new]];
	}
	else
	{
		NSString *tmp = [NSString stringWithCString:reinterpret_cast<const char *>(title.ToUTF8().GetRawZeroTerminatedData()) encoding:NSUTF8StringEncoding];
		[this->cocoaGroupBox setTitle:tmp];
		[tmp release];
	}
}

//Private methods
StdXX::Math::SizeD CocoaGroupBoxBackend::ComputeTextSize(NSString *string, NSFont *font) const
{
	NSDictionary *attributes = @{NSFontAttributeName: font};
	NSSize s = [string sizeWithAttributes:attributes];

	//[attributes release];

	return StdXX::Math::SizeD(s.width, s.height);
}








//OLD STUFF

void CocoaGroupBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CocoaGroupBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}