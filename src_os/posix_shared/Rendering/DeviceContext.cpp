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
//Class header
#include <Std++/Rendering/DeviceContext.hpp>
//Global
//#include <gdk/gdkx.h>
//#include <GL/glx.h>
//Local
#include <Std++/Integer.hpp>
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
//#include "../UI/Gtk.h"
//#include "../UI/Displays/_GtkOpenGLWidget.h"
#include "../../../src_backends/OpenGL3Core/GL3Functions.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;
//Definitions
#define THIS ((X11_DisplayAndWindow *)this->systemHandle)

/*
struct X11_DisplayAndWindow
{
	Display *display;
	::Window windowId;
};
*/

//Local functions
/*
GLXFBConfig ChooseFrameBufferConfig(Display *display, uint8 nSamples)
{
	static const int frameBufferConfigAttributes[] =
		{
			GLX_X_RENDERABLE,	True,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			GLX_DEPTH_SIZE,		24,
			GLX_STENCIL_SIZE,	8,
			GLX_DOUBLEBUFFER,	True,
			GLX_SAMPLE_BUFFERS,	(nSamples ? 1 : 0),
			GLX_SAMPLES,		nSamples,
			None
		};

	int nConfigs;
	GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), frameBufferConfigAttributes, &nConfigs);

	//Choose an FB-Config that has the closest number of samples as desired

	int bestIndex = 0, bestDiff = Integer<int>::Max();
	for(int i = 0; i < nConfigs; i++)
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
		if(vi)
		{
			int cfg_nSamples;
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &cfg_nSamples);

			int diff = ABS((int) nSamples - cfg_nSamples);
			if (diff <= bestDiff)
			{
				bestDiff = diff;
				bestIndex = i;

				printf("choosing %d\n", bestIndex);
				fflush(stdout);
			}

			XFree(vi);
		}
	}
	GLXFBConfig bestFB = fbc[bestIndex];

	XFree(fbc);

	return bestFB;
}
 */

//Private methods
void DeviceContext::DestroyOSContext()
{
	//glXDestroyContext(display, this->deviceState);
}

void DeviceContext::UnbindOSContext()
{
	NOT_IMPLEMENTED_ERROR; //TODO: next line was correct implementation
	//gdk_gl_context_clear_current();
	//glXMakeCurrent(display, 0, 0);
}

//Public methods
void DeviceContext::SwapBuffers()
{
	//glXSwapBuffers(display, win);
}