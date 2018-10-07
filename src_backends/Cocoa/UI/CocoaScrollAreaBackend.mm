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
#include "CocoaScrollAreaBackend.hh"
//Local
#import "CocoaContainerBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
void CocoaScrollAreaBackend::AddChild(Widget *widget)
{
	CocoaWidgetBackend *cocoaView = dynamic_cast<CocoaWidgetBackend *>(widget->_GetBackend());
	[[this->cocoaScrollView contentView] setDocumentView:cocoaView->GetView()];
}

WidgetContainerBackend *CocoaScrollAreaBackend::CreateContentAreaBackend(CompositeWidget &widget)
{
	return new CocoaContainerBackend(this->GetUIBackend(), widget, [NSView new]);
}

RectD CocoaScrollAreaBackend::GetContentAreaBounds() const
{
	SizeD contentSize = this->scrollArea.GetContentContainer()->GetSizeHint();
	SizeD scrollAreaSize = this->GetWidget().GetSize();

	SizeD allocationSize = contentSize.Max(scrollAreaSize);

	return RectD(PointD(), allocationSize);
}

SizeD CocoaScrollAreaBackend::GetSizeHint() const
{
	//TODO: sizes of scroll bars
	return StdXX::Math::SizeD();
}

NSView *CocoaScrollAreaBackend::GetView()
{
	return this->cocoaScrollView;
}

Widget &CocoaScrollAreaBackend::GetWidget()
{
	return this->scrollArea;
}

const Widget &CocoaScrollAreaBackend::GetWidget() const
{
	return this->scrollArea;
}






//NOT IMPLEMENTED
void _stdxx_::CocoaScrollAreaBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaScrollAreaBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}