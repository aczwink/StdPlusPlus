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
//Global
#undef new
#include <Cocoa/Cocoa.h>
//Local
#include "../../OpenGL3Core/Rendering/OpenGLDeviceContext.hpp"

namespace _stdpp
{
	class CocoaOpenGL3CoreDeviceContext : public OpenGLDeviceContext
	{
	public:
		//Constructor
		CocoaOpenGL3CoreDeviceContext(NSOpenGLView *openGLView, uint8 nSamples, GL_EXT_LOADER loader);

		//Destructor
		~CocoaOpenGL3CoreDeviceContext()
		{
			[this->openGLContext release];
			[this->pixelFormat release];
		}

	protected:
		//Methods
		void Bind() const override
		{
			[this->openGLContext makeCurrentContext];
		}

	private:
		//Members
		NSOpenGLPixelFormat *pixelFormat;
		NSOpenGLContext *openGLContext;
	};
}