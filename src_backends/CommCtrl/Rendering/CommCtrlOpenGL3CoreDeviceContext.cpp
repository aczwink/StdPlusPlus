/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "CommCtrlOpenGL3CoreDeviceContext.hpp"

//Namespaces
using namespace _stdxx_;

//Constructor
CommCtrlOpenGL3CoreDeviceContext::CommCtrlOpenGL3CoreDeviceContext(HDC hDC, uint8 nSamples, GL_EXT_LOADER loader) : hDC(hDC)
{
	//firstly set correct pixel format in device context

	int32 attributeList[] =
	{
		//support for opengl rendering
		WGL_SUPPORT_OPENGL_ARB,
		true,

		//support for rendering to window
		WGL_DRAW_TO_WINDOW_ARB,
		true,

		//support for hardware acceleration
		WGL_ACCELERATION_ARB,
		WGL_FULL_ACCELERATION_ARB,

		//support for 24bit color
		WGL_COLOR_BITS_ARB,
		24,

		//support for 24bit depth buffer
		WGL_DEPTH_BITS_ARB,
		24,

		//support for double buffering
		WGL_DOUBLE_BUFFER_ARB,
		true,

		//support for swap chain
		WGL_SWAP_METHOD_ARB,
		WGL_SWAP_EXCHANGE_ARB,

		//support for rgba pixel type
		WGL_PIXEL_TYPE_ARB,
		WGL_TYPE_RGBA_ARB,

		//support for 8 but stencil buffer
		WGL_STENCIL_BITS_ARB,
		8,

		//support for MSAA
		WGL_SAMPLE_BUFFERS_ARB,
		((nSamples > 1) ? 1 : 0),
		WGL_SAMPLES_ARB,
		((nSamples > 1) ? nSamples : 0),

		0 //zero terminate
	};

	//query for a pixel format that matches attributes best
	int32 pixelFormatIdx;
	UINT nPixelFormatsReturned;
	wglChoosePixelFormatARB(hDC, attributeList, nullptr, 1, &pixelFormatIdx, &nPixelFormatsReturned);

	//set the pixel format
	PIXELFORMATDESCRIPTOR pfd;
	SetPixelFormat(hDC, pixelFormatIdx, &pfd);

	//second create openGL 3.3 context
	static const int32 contextAttributeList[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,
		3,

		WGL_CONTEXT_MINOR_VERSION_ARB,
		3,

		0 //zero terminate
	};

	this->hGLRC = wglCreateContextAttribsARB(hDC, 0, contextAttributeList);

	this->Init(loader);
}

//Destructor
CommCtrlOpenGL3CoreDeviceContext::~CommCtrlOpenGL3CoreDeviceContext()
{
	HWND hWnd = WindowFromDC(this->hDC);

	wglMakeCurrent(nullptr, nullptr);	
	wglDeleteContext(this->hGLRC);
	ReleaseDC(hWnd, this->hDC);
}

//Public methods
void CommCtrlOpenGL3CoreDeviceContext::SwapBuffers()
{
	::SwapBuffers(this->hDC);
}

//Protected methods
void CommCtrlOpenGL3CoreDeviceContext::MakeContextCurrent() const
{
	wglMakeCurrent(this->hDC, this->hGLRC);
}