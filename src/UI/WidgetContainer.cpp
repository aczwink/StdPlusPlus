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
#include <Std++/UI/WidgetContainer.hpp>
//Local
#include <Std++/UI/Window.hpp>
#include <Std++/UI/Layouts/GridLayout.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

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
    this->backend->Paint();
    if(!this->backend)
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
	if(this->backend)
		return this->backend->GetChildrenRect();
	return {Point(), this->GetSize()};
}

ERenderMode WidgetContainer::GetChildrenRenderMode() const
{
    return this->GetWindow()->GetRenderMode();
}

Size WidgetContainer::GetSizeHint() const
{
	Size size;
	if(this->backend)
		size = size.Max(this->backend->GetSizeHint());
    if(this->layout)
		size = size.Max(this->layout->GetPreferredSize(*this));

	return size;
}