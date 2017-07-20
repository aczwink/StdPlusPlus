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
#include <ACStdLib/UI/WidgetContainer.hpp>
//Local
#include <ACStdLib/UI/Window.hpp>
#include <ACStdLib/UI/Layouts/GridLayout.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
WidgetContainer::WidgetContainer(WidgetContainer *pContainer) : Widget(pContainer)
{
    this->layout = new GridLayout;
    if(pContainer)
        this->renderMode = pContainer->renderMode;
}

//Destructor
WidgetContainer::~WidgetContainer()
{
    while(!this->children.IsEmpty())
        //don't use PopFront, because child will delete itself from the children list
        //this way the child will find itself at the head of the list and removing will be fast
        //if we remove the child before, the child will loop through the whole list in order to (vainly) find itself
        delete this->children.GetFront();

    if(this->layout)
        delete this->layout;
}

//Eventhandlers
void WidgetContainer::OnPaint()
{
    if(this->systemHandle)
    {
        this->IgnoreEvent();
    }
    else
    {
        for(Widget *const& refpChild : this->children)
            refpChild->Repaint();
    }
}

void WidgetContainer::OnResized()
{
    this->layout->Layout(*this);
}

void WidgetContainer::SetLayout(ILayout *pLayout)
{
    if(this->layout)
        delete this->layout;

    this->layout = pLayout;
}

//Public methods
Rect WidgetContainer::GetChildrenRect() const
{
	return Rect(Point(0, 0), this->GetSize());
}

ERenderMode WidgetContainer::GetChildrenRenderMode() const
{
    return this->GetWindow()->GetRenderMode();
}

Size WidgetContainer::GetSizeHint() const
{
    if(this->layout)
        return this->layout->GetPreferredSize(*this);

    return Size();
}