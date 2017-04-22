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
#include "../../headers/UI/AWidgetContainer.h"
//Local
#include "../../headers/UI/Window.h"
#include "../../headers/UI/Layouts/CGridLayout.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
AWidgetContainer::AWidgetContainer(AWidgetContainer *pContainer) : AWidget(pContainer)
{
    this->pLayout = new CGridLayout;
    if(pContainer)
        this->renderMode = pContainer->renderMode;
}

//Destructor
AWidgetContainer::~AWidgetContainer()
{
    for(const AWidget *const& refpChild : this->children)
    {
        delete refpChild;
    }

    if(this->pLayout)
        delete this->pLayout;
}

//Eventhandlers
void AWidgetContainer::OnPaint()
{
    if(this->pOSHandle)
    {
        this->IgnoreEvent();
    }
    else
    {
        for(AWidget *const& refpChild : this->children)
            refpChild->Repaint();
    }
}

void AWidgetContainer::OnResized()
{
    this->pLayout->Layout(*this);
}

void AWidgetContainer::SetLayout(ILayout *pLayout)
{
    if(this->pLayout)
        delete this->pLayout;

    this->pLayout = pLayout;
}

//Public methods
ERenderMode AWidgetContainer::GetChildrenRenderMode() const
{
    return this->GetWindow()->GetRenderMode();
}

CSize AWidgetContainer::GetSizeHint() const
{
    if(this->pLayout)
        return this->pLayout->GetPreferredSize(*this);

    return CSize();
}