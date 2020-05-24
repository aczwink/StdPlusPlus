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
//Global
#include <gdk/gdk.h>
//Local
#include "../../OpenGL3Core/Rendering/OpenGLDeviceContext.hpp"
#include "../UI/oldGtkWindowBackend.hpp"
#include "../UI/Gtk3WidgetBackend.hpp"
//Namespaces
using namespace StdXX::Rendering;
using namespace StdXX::UI;

namespace _stdxx_
{
	class GtkOpenGLDeviceContext : public OpenGLDeviceContext
	{
	public:
		//Constructor
		GtkOpenGLDeviceContext(Gtk3WidgetBackend& widgetBackend, GL_EXT_LOADER loader);

		//Methods
		void EnableDepthTest(bool enabled) override;
		void ResetDepthTest();
		void SwapBuffers() override;

	protected:
		//Methods
		void MakeContextCurrent() const override;

	private:
		//Members
		GdkGLContext *gdkGLContext;
		uint32 screenFrameBufferId;
		bool depthTestState;
	};
}