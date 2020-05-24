/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
//Local
#include <Std++/Rendering/DeviceContext.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
RenderTargetWidget::RenderTargetWidget(const WidgetFrameBufferSetup& frameBufferSetup) : deviceContext(nullptr), frameBufferSetup(frameBufferSetup)
{
    this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
    this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Expanding);
}

//Destructor
RenderTargetWidget::~RenderTargetWidget()
{
	delete this->deviceContext;
}

//Private methods
void RenderTargetWidget::RealizeSelf()
{
	_stdxx_::WidgetBackend* widgetBackend = this->_GetUIBackend()->CreateRenderTargetWidgetBackend(*this);

	this->_SetBackend(widgetBackend);
}

//Eventhandlers
void RenderTargetWidget::OnPaint(PaintEvent& event)
{
    this->deviceContext->ClearColorBuffer(Color());
    this->deviceContext->SwapBuffers();

	event.Accept();
}

void RenderTargetWidget::OnRealized()
{
	Widget::OnRealized();
	this->deviceContext = this->_GetUIBackend()->renderBackends.GetActiveBackend()->CreateDeviceContext(*this->_GetBackend());
}

void RenderTargetWidget::OnResized()
{
	if(this->deviceContext)
	{
		Math::SizeD size = this->GetSize();
		this->deviceContext->SetViewPort(size.width, size.height);
	}
}