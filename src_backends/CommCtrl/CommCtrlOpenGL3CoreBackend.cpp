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
#include "CommCtrlOpenGL3CoreBackend.hpp"
//Local
#include "Rendering/CommCtrlOpenGL3CoreDeviceContext.hpp"
#include "Rendering/WGL.h"
#include "UI/Win32Window.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
Rendering::DeviceContext *CommCtrlOpenGL3CoreBackend::CreateDeviceContext(_stdxx_::WidgetBackend &backend, uint8 nSamples) const
{
	this->LoadWGLFunctions();

	Win32Window &wnd = dynamic_cast<Win32Window &>(backend);
	return new _stdxx_::CommCtrlOpenGL3CoreDeviceContext(GetDC(wnd.GetHWND()), nSamples, this->LoadWindowSystemOpenGLExtension);
}

//Private methods
void CommCtrlOpenGL3CoreBackend::LoadWGLFunctions() const
{
	if (this->wglFunctionsLoaded)
		return;
	
	//step 1: we need to have a window to initialize WGL functions
	HWND hFakeWnd = CreateWindowW(STDPLUSPLUS_WIN_WNDCLASS, nullptr, WS_POPUP, 0, 0, 640, 480, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	HDC hDC = GetDC(hFakeWnd);

	//set any pixel format
	PIXELFORMATDESCRIPTOR pfd;
	SetPixelFormat(hDC, 1, &pfd);

	//get opengl 1.1 context
	HGLRC hGLContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLContext);

	//load function pointers
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress(u8"wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress(u8"wglCreateContextAttribsARB");

	//clean up
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hGLContext);
	ReleaseDC(hFakeWnd, hDC);
	DestroyWindow(hFakeWnd);

	this->wglFunctionsLoaded = true;
}