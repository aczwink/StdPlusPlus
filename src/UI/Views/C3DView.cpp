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
#include <ACStdLib/UI/Views/C3DView.h>
//Local
#include <ACStdLib/Rendering/CDeviceContext.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Prototypes
void LoadOpenGL();

//Constructor
C3DView::C3DView(AWidgetContainer *pParent) : AWidgetContainer(pParent)
{
    this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
    this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Expanding);

    this->CreateOSWindow();

    LoadOpenGL();

    this->pDeviceContext = new Rendering::CDeviceContext(*this, 4);
}

//Destructor
C3DView::~C3DView()
{
    delete this->pDeviceContext;
}

//Eventhandlers
void C3DView::OnPaint()
{
    this->pDeviceContext->ClearColorBuffer(CColor());

    this->pDeviceContext->SwapBuffers();
}

void C3DView::OnResized()
{
    CSize size;

    size = this->GetSize();

    this->pDeviceContext->SetViewPort(size.width, size.height);
}