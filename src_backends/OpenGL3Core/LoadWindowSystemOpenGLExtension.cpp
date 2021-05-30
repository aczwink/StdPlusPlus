/*
 * Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include "OpenGL3CoreBackend.hpp"
//Local
#include <Std++/Containers/Strings/StringUtil.h>
//Namespaces
using namespace StdXX;

#ifdef _STDPP_WINDOWSYSTEM_COCOA
#include <mach-o/dyld.h>
#endif
#ifdef _STDPP_WINDOWSYSTEM_WINDOWS
#include <Windows.h>
#endif
#ifdef _STDPP_WINDOWSYSTEM_X11
#include <GL/glx.h>
#endif

//Class functions
void *OpenGL3CoreBackend::LoadWindowSystemOpenGLExtension(const char8_t* extensionName)
{
#ifdef _STDPP_WINDOWSYSTEM_COCOA
	uint32 len = GetStringLength(extensionName);
	char *symbolName = static_cast<char *>(malloc(len + 2));
	MemCopy(symbolName + 1, extensionName, len + 1);
	symbolName[0] = u8'_';
	NSSymbol symbol = nullptr;
	if (NSIsSymbolNameDefined(symbolName))
		symbol = NSLookupAndBindSymbol(symbolName);
	free(symbolName);
	if (symbol)
		return NSAddressOfSymbol(symbol);
#endif
#ifdef _STDPP_WINDOWSYSTEM_WINDOWS
	void *p = wglGetProcAddress(extensionName);
	if (!p)
	{
		static HMODULE hOGLModule = LoadLibraryA(u8"opengl32.dll");
		return GetProcAddress(hOGLModule, extensionName);
	}
	return p;
#endif
#ifdef _STDPP_WINDOWSYSTEM_X11
	return (void *)glXGetProcAddressARB((const GLubyte *)extensionName);
#endif

	NOT_IMPLEMENTED_ERROR; //TODO: trouble!
	return nullptr;
}