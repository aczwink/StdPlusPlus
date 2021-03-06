/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "CocoaOpenGL3CoreBackend.hh"
//Local
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#import "Rendering/CocoaOpenGL3CoreDeviceContext.hh"
#import "UI/CocoaRenderTargetWidgetBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Rendering;

//Public methods
DeviceContext *CocoaOpenGL3CoreBackend::CreateDeviceContext(WidgetBackend &backend) const
{
    UI::RenderTargetWidget& renderTargetWidget = dynamic_cast<UI::RenderTargetWidget &>(backend.GetWidget());
    const WidgetFrameBufferSetup &frameBufferSetup = renderTargetWidget.FrameBufferSetup();

    CocoaRenderTargetWidgetBackend &renderTargetWidgetBackend = dynamic_cast<CocoaRenderTargetWidgetBackend &>(backend);
    return new CocoaOpenGL3CoreDeviceContext(renderTargetWidgetBackend.GetOpenGLView(), frameBufferSetup.nSamples, this->LoadWindowSystemOpenGLExtension);
}
