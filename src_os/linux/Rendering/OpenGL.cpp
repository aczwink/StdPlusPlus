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
//Global
#include <gdk/gdkx.h>
#include <GL/glx.h>
//Local
#include <ACStdLib/Debug.h>
#include "../UI/Gtk.h"

//Prototypes
void LoadOpenGLExtensions(void *(*pLoadFunction)(const char *extensionName));

static void *LoadExtension(const char *extensionName)
{
	return (void *)glXGetProcAddressARB((const GLubyte *)extensionName);
}

void LoadOpenGL()
{
	static bool loadedOpenGL = false;

	if(loadedOpenGL)
		return;

	//We only support X11 currently
	GdkDisplay *display = gdk_display_manager_get_default_display(gdk_display_manager_get());
	ASSERT(GDK_IS_X11_DISPLAY(display));

	LoadOpenGLExtensions(LoadExtension);

	loadedOpenGL = true;
}