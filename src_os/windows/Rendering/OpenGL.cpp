/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <Windows.h>
//Local
#include "../../../src_backends/OpenGL/GLFunctions.h"
#include <Std++/Debug.h>
#include "OpenGL.h"

//WGL Function pointers
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

//Global variables
HMODULE g_hOGLModule;

void LoadOpenGLExtensions(void *(*pLoadFunction)(const char *pExtension));

//Local functions
void *OpenGL_LoadExtensionWindows(const char *pExtension)
{
	void *p;

	p = wglGetProcAddress(pExtension);
	if(!p)
		return GetProcAddress(g_hOGLModule, pExtension);

	return p;
}

static void LoadPlatformExtensions()
{
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
}

static void InitExtensions(HWND hWnd)
{
	HDC hDC;
	HGLRC hGLContext;
	PIXELFORMATDESCRIPTOR pfd;

	hDC = GetDC(hWnd);

	//load extensions opengl32.dll
	g_hOGLModule = LoadLibraryA("opengl32.dll");

	//set any pixel format
	SetPixelFormat(hDC, 1, &pfd);

	//get opengl 1.1 context
	hGLContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hGLContext);

	//load extensions
	LoadPlatformExtensions();
	NOT_IMPLEMENTED_ERROR; //TODO: next line
	//LoadOpenGLExtensions(OpenGL_LoadExtensionWindows);

	//clean up
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hGLContext);
	ReleaseDC(hWnd, hDC);
}

void LoadOpenGL()
{
	static bool loadedOpenGL = false;

	if(loadedOpenGL)
		return;

	HWND hFakeWnd;

	//step 1: we need to have a window to initialize OpenGL extensions
    NOT_IMPLEMENTED_ERROR; //TODO: next line
	//hFakeWnd = CreateWindowW(STDPLUSPLUS_WIN_WNDCLASS, nullptr, WS_POPUP, 0, 0, 640, 480, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	InitExtensions(hFakeWnd);
	DestroyWindow(hFakeWnd);

	loadedOpenGL = true;
}