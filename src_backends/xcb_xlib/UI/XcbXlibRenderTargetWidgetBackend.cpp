/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "XcbXlibRenderTargetWidgetBackend.hpp"
//Global
#include <GL/glx.h>
//X layer
#include "../XLayer/XcbWindow.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
XcbXlibRenderTargetWidgetBackend::XcbXlibRenderTargetWidgetBackend(UIBackend &uiBackend, XConnection& xConnection, UI::RenderTargetWidget &renderTargetWidget)
	: WidgetBackend(uiBackend),
	xConnection(xConnection), renderTargetWidget(renderTargetWidget)
{
	this->xcbWindow = new XcbWindow(xConnection, this->SelectVisual());
}

//Public methods
const UI::Widget &XcbXlibRenderTargetWidgetBackend::GetWidget() const
{
	return this->renderTargetWidget;
}

void XcbXlibRenderTargetWidgetBackend::Show(bool visible)
{
	this->xcbWindow->Map(visible);
}




//NOT IMPLEMENTED
StdXX::Math::SizeD _stdxx_::XcbXlibRenderTargetWidgetBackend::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void _stdxx_::XcbXlibRenderTargetWidgetBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibRenderTargetWidgetBackend::SetBounds(const StdXX::Math::RectD &bounds)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibRenderTargetWidgetBackend::SetEnabled(bool enable)
{
	ASSERT(enable, u8"IMPLEMENT THIS"); //TODO: implement me
}

void _stdxx_::XcbXlibRenderTargetWidgetBackend::SetHint(const StdXX::String &text)
{
	ASSERT(text.IsEmpty(), u8"IMPLEMENT THIS"); //TODO: implement me
}
//END OF NOT IMPLEMENTED

//Private methods
int XcbXlibRenderTargetWidgetBackend::SelectVisual() const
{
	const WidgetFrameBufferSetup& frameBufferSetup = this->renderTargetWidget.FrameBufferSetup();
	static const int visual_attribs[] =
	{
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_STENCIL_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		GLX_SAMPLE_BUFFERS, uint8((frameBufferSetup.nSamples > 1) ? 1 : 0),
		GLX_SAMPLES, uint8((frameBufferSetup.nSamples > 1) ? frameBufferSetup.nSamples : 0),
		None
	};

	//find a matching framebuffer config
	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(this->xConnection.GetDisplay(), DefaultScreen(this->xConnection.GetDisplay()), visual_attribs, &fbcount);

	GLXFBConfig frameBufferConfig;
	return 0;
}