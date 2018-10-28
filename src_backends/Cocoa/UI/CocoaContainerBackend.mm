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
#include "CocoaContainerBackend.hh"
//Local
#include <Std++/UI/Layouts/ILayout.h>
#import "CocoaEventSource.hh"
#import "CocoaSpinBoxBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void CocoaContainerBackend::AddChild(StdXX::UI::Widget *widget)
{
	CocoaSpinBoxBackend *spinBoxBackend = dynamic_cast<CocoaSpinBoxBackend *>(widget->_GetBackend());
	if(spinBoxBackend)
	{
		[this->view addSubview:spinBoxBackend->GetTextField()];
		[this->view addSubview:spinBoxBackend->GetStepper()];
		return;
	}

	CocoaWidgetBackend *cocoaView = dynamic_cast<CocoaWidgetBackend *>(widget->_GetBackend());
	if(cocoaView) //some widgets don't need a backend
		[this->view addSubview:cocoaView->GetView()];
}

UI::Widget &CocoaContainerBackend::GetWidget()
{
	return this->widgetContainer;
}

const StdXX::UI::Widget &CocoaContainerBackend::GetWidget() const
{
	return this->widgetContainer;
}

void CocoaContainerBackend::SetBounds(const StdXX::Math::RectD &area)
{
	[this->view setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height())];
}

void CocoaContainerBackend::SetEnabled(bool enable)
{
	//TODO: implement me
}








//OLD STUFF
void _stdxx_::CocoaContainerBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaContainerBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Math::SizeD CocoaContainerBackend::GetSizeHint() const
{
	return StdXX::Math::SizeD();
}