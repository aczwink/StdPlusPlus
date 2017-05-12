/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/UI/Widget.hpp>
//Local
#include <ACStdLib/UI/WidgetContainer.h>
#include <ACStdLib/UI/Window.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
Widget::Widget(WidgetContainer *pContainer)
{
    this->pParent = pContainer;
    if(pContainer && pContainer->pOwner)
        this->pOwner = pContainer->pOwner;
    else
    {
        ASSERT(!pContainer || IS_INSTANCE_OF(pContainer, Window));
        this->pOwner = (Window *)pContainer;
    }
    this->systemHandle = nullptr;
    if(pContainer)
        pContainer->children.InsertTail(this);
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

void Widget::OnPaint()
{
    this->IgnoreEvent();
}

void Widget::OnResized()
{
    this->IgnoreEvent();
}

//Protected methods
ERenderMode Widget::GetRenderMode() const
{
    return this->pParent->GetRenderMode();
}

//Public methods
Size Widget::GetSizeHint() const
{
    return Size();
}

void Widget::SetRect(const Rect &area)
{
    this->bounds = area;

	if(this->systemHandle)
		this->System_SetRect(area);

	this->OnResized();
}

Point Widget::TransformToWindow(const Point &refPoint) const
{
    WidgetContainer *pParent;
    Point transformed;
    Rect rcParent;

    pParent = this->pParent;
    transformed = refPoint + this->bounds.origin;
    while(pParent != this->pOwner)
    {
        rcParent = pParent->bounds;

        transformed.x += rcParent.x();
        transformed.y += rcParent.y();

        pParent = pParent->pParent;
    }

    return transformed;
}