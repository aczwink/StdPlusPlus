/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "GtkOpenGLDeviceContext.hpp"
//Namespaces
using namespace _stdxx_;

//Constructor
GtkOpenGLDeviceContext::GtkOpenGLDeviceContext(Gtk3WidgetBackend& widgetBackend, GL_EXT_LOADER loader)
	: depthTestState(false) //initially, depth testing is disabled in OpenGL
{
	GtkWidget *gtkWidget = widgetBackend.GetGtkWidget();
	GtkGLArea *gtkGLArea = GTK_GL_AREA(gtkWidget);

	gtk_widget_realize(gtkWidget); //important!!!

	this->gdkGLContext = gtk_gl_area_get_context(gtkGLArea);
	ASSERT(this->gdkGLContext, u8"TODO: IMPLEMENT ME");
	this->Init(loader);

	this->Bind();
	gtk_gl_area_attach_buffers(gtkGLArea); //also important so that device context can read default frame buffer
	this->glFuncs.glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint *>(&this->screenFrameBufferId));
}

//Public methods
void GtkOpenGLDeviceContext::EnableDepthTest(bool enabled)
{
	OpenGLDeviceContext::EnableDepthTest(enabled);
	/*
	GL_DEPTH_TEST is enabled/disabled by GtkGlArea on each render, by whether the frame buffer has a depth buffer
	work around that we save the depth state set to this device context, and then reset it before each paint event
	 */
	this->depthTestState = enabled;
}

void GtkOpenGLDeviceContext::ResetDepthTest()
{
	//the original depth test is reset here
	this->EnableDepthTest(this->depthTestState);
}

void GtkOpenGLDeviceContext::SwapBuffers()
{
}

//Protected methods
void GtkOpenGLDeviceContext::MakeContextCurrent() const
{
	gdk_gl_context_make_current(this->gdkGLContext);
}