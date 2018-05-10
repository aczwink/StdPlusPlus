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
#include <Std++/UI/Widget.hpp>
//Local
#include <Std++/UI/WidgetContainer.hpp>
#include <Std++/UI/Window.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Constructor
Widget::Widget(WidgetContainer *parent)
{
    this->parent = parent;
    if(parent && parent->owner)
        this->owner = parent->owner;
    else
    {
        ASSERT(!parent || IS_INSTANCE_OF(parent, Window), "A widget must have a parent or it must be a window itself");
        this->owner = (Window *)parent;
    }
    this->backend = nullptr;
    if(parent)
        parent->children.InsertTail(this);
}

//Destructor
Widget::~Widget()
{
    if(this->parent)
        this->parent->RemoveChild(this);

    if(this->backend)
        delete this->backend;
}

//Public methods
Size Widget::GetSizeHint() const
{
	if(this->backend)
		return this->backend->GetSizeHint();

	return Size();
}

Point Widget::TranslateToAncestorCoords(const Point &point, const WidgetContainer *ancestor) const
{
	if(this == ancestor)
		return point;

	Point translated = this->TranslateToParentCoords(point);
	const WidgetContainer *current = this->GetParent();
	while(current != ancestor)
	{
		translated = current->TranslateChildToWidgetCoords(translated);
		translated = current->TranslateToParentCoords(translated);
		current = current->parent;
	}
	return translated;
}

//Eventhandlers
void Widget::OnMouseButtonPressed(MouseButton button, const Point &pos)
{
	this->IgnoreEvent();
}

void Widget::OnMouseButtonReleased(MouseButton button, const Point &pos)
{
	this->IgnoreEvent();
}

void Widget::OnMouseMoved(const Point &pos)
{
	this->IgnoreEvent();
}

void Widget::OnMouseWheelTurned(int16 delta)
{
	this->IgnoreEvent();
}

void Widget::OnPaint()
{
    this->backend->Paint();
}

void Widget::OnResized()
{
    this->IgnoreEvent();
}

void Widget::OnResizing(const Rect &newBounds)
{
	this->bounds = newBounds;
	if(this->backend)
		this->backend->SetBounds(newBounds);
	this->OnResized();
}