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
#include <ACStdLib/UI/Controls/RenderTargetWidget.hpp>
//Local
#include <ACStdLib/Rendering/DeviceContext.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Prototypes
void LoadOpenGL();

//Constructor
RenderTargetWidget::RenderTargetWidget(WidgetContainer *pParent) : WidgetContainer(pParent)
{
    this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
    this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Expanding);

    this->CreateOSHandle();

    LoadOpenGL();

    this->deviceContext = new Rendering::DeviceContext(*this, 4);
}

//Destructor
RenderTargetWidget::~RenderTargetWidget()
{
	this->System_Destroy();

    delete this->deviceContext;
}

//Eventhandlers
void RenderTargetWidget::OnPaint()
{
    this->deviceContext->ClearColorBuffer(Color());

    this->deviceContext->SwapBuffers();
}

void RenderTargetWidget::OnResized()
{
    Size size;

    size = this->GetSize();

    this->deviceContext->SetViewPort(size.width, size.height);
}