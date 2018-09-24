/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Containers/ContentAreaWidget.hpp>
//Namespaces
using namespace StdXX::Math;
using namespace StdXX::UI;

//Destructor
ContentAreaWidget::~ContentAreaWidget()
{
	delete this->contentContainer;
}

//Public methods
Widget *ContentAreaWidget::GetChild(uint32 index)
{
	ASSERT(index == 0, u8"A content area widget has just one child");
	return this->contentContainer;
}

const Widget* ContentAreaWidget::GetChild(uint32 index) const
{
	ASSERT(index == 0, u8"A content area widget has just one child");
	return this->contentContainer;
}

uint32 ContentAreaWidget::GetNumberOfChildren() const
{
	if (this->contentContainer == nullptr)
		return 0;
	return 1;
}

SizeD ContentAreaWidget::GetSizeHint() const
{
	return Widget::GetSizeHint() + this->contentContainer->GetSizeHint();
}

void ContentAreaWidget::RemoveChild(Widget *child)
{
	ASSERT(this->contentContainer == child, u8"Can't remove child from ContentAreaWidget, if its content area is not the widget.");
	this->contentContainer = nullptr;
	this->FreeWidgetOwnership(child);
}

//Eventhandlers
void ContentAreaWidget::OnResized()
{
	this->contentContainer->SetBounds(this->contentAreaWidgetBackend->GetContentAreaBounds());
}
