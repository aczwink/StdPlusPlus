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
#include "../../headers/UI/AWidget.h"
//Local
#include "../../headers/UI/AWidgetContainer.h"
#include "../../headers/UI/Window.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
AWidget::AWidget(AWidgetContainer *pContainer)
{
    this->pParent = pContainer;
    if(pContainer && pContainer->pOwner)
        this->pOwner = pContainer->pOwner;
    else
    {
        ASSERT(!pContainer || IS_INSTANCE_OF(pContainer, Window));
        this->pOwner = (Window *)pContainer;
    }
    this->pOSHandle = nullptr;
    if(pContainer)
        pContainer->children.InsertTail(this);
}

//Eventhandlers
void AWidget::OnPaint()
{
    this->IgnoreEvent();
}

void AWidget::OnResized()
{
    this->IgnoreEvent();
}

//Protected methods
ERenderMode AWidget::GetRenderMode() const
{
    return this->pParent->GetRenderMode();
}

//Public methods
CSize AWidget::GetSize() const
{
    if(this->pOSHandle == nullptr)
        return this->bounds.size;

    return this->GetOSSize();
}

CSize AWidget::GetSizeHint() const
{
    return CSize();
}

CPoint AWidget::TransformToWindow(const CPoint &refPoint) const
{
    AWidgetContainer *pParent;
    CPoint transformed;
    CRect rcParent;

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