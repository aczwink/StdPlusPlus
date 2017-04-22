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
#include <ACStdLib/UI/Views/CTreeView.h>
//Local
#include <ACStdLib/UI/AWidgetContainer.h>
#include <ACStdLib/UI/Controllers/ATreeController.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
CTreeView::CTreeView(AWidgetContainer *pParent) : AWidget(pParent)
{
    this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
    this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Expanding);

    this->pController = nullptr;

    this->CreateOSWindow();
}

//Eventhandlers
void CTreeView::OnSelectionChanged()
{
    this->pController->OnSelectionChanged();
}

//Public methods
void CTreeView::SetController(ATreeController &refController)
{
    this->pController = &refController;
    this->pController->pTreeView = this;

    this->OnModelChanged();
}