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
#include <ACStdLib/UI/WidgetContainer.h>
//Local
#include <ACStdLib/UI/Window.h>
#include <ACStdLib/UI/Layouts/GridLayout.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
WidgetContainer::WidgetContainer(WidgetContainer *pContainer) : Widget(pContainer)
{
    this->pLayout = new GridLayout;
    if(pContainer)
        this->renderMode = pContainer->renderMode;
}

//Destructor
WidgetContainer::~WidgetContainer()
{
    for(const Widget *const& refpChild : this->children)
    {
        delete refpChild;
    }

    if(this->pLayout)
        delete this->pLayout;
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
    this->pLayout->Layout(*this);
}

void WidgetContainer::SetLayout(ILayout *pLayout)
{
    if(this->pLayout)
        delete this->pLayout;

    this->pLayout = pLayout;
}

//Public methods
Rect WidgetContainer::GetChildrenRect() const
{
	return Rect(CPoint(0, 0), this->GetSize());
}

ERenderMode WidgetContainer::GetChildrenRenderMode() const
{
    return this->GetWindow()->GetRenderMode();
}

Size WidgetContainer::GetSizeHint() const
{
    if(this->pLayout)
        return this->pLayout->GetPreferredSize(*this);

    return Size();
}