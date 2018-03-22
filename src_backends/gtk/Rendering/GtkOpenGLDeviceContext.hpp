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
//Local
#include "../../OpenGL/Rendering/OpenGLDeviceContext.hpp"
#include "../UI/GtkWindowBackend.hpp"
//Namespaces
using namespace StdPlusPlus::Rendering;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
	class GtkOpenGLDeviceContext : public OpenGLDeviceContext
	{
	public:
		//Constructor
		GtkOpenGLDeviceContext(const GtkWindowBackend &view, uint8 nSamples)
		{
			GtkWidget *gtkWidget = view.GetGtkWidget();
			GtkGLArea *gtkGLArea = GTK_GL_AREA(gtkWidget);

			gtk_widget_realize(gtkWidget); //important!!!
			gtk_gl_area_attach_buffers(gtkGLArea); //also important so that device context can read default frame buffer

			this->gdkGLContext = gtk_gl_area_get_context(gtkGLArea);
			//this->deviceState = ac_gtk_opengl_widget_get_context(AC_GTK_OPENGL_WIDGET(PRIVATE_DATA(&renderTargetWidget)->widget));

			this->Bind();
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint *>(&this->screenFrameBufferId));

			this->Init();
		}

	protected:
		//Methods
		void Bind() const override
		{
			gdk_gl_context_make_current(this->gdkGLContext);
		}

	private:
		//Members
		GdkGLContext *gdkGLContext;
		uint32 screenFrameBufferId;
	};
}