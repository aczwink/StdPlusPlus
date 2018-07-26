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
#include "CocoaOpenGL3CoreDeviceContext.hh"
//Namespaces
using namespace _stdxx_;

//Constructor
CocoaOpenGL3CoreDeviceContext::CocoaOpenGL3CoreDeviceContext(NSOpenGLView *openGLView, uint8 nSamples, GL_EXT_LOADER loader)
{
	NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
	{
		//profile and version
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,

		//hardware acceleration
		NSOpenGLPFAAccelerated,

		//color buffer
		NSOpenGLPFAColorSize, 24,

		//depth buffer
		NSOpenGLPFADepthSize, 24,

		//double buffering
		NSOpenGLPFADoubleBuffer,

		//stencil buffer
		NSOpenGLPFAStencilSize, 8,

		//multi sampling
		NSOpenGLPFASampleBuffers, uint8((nSamples > 1) ? 1 : 0),
		NSOpenGLPFASamples, uint8((nSamples > 1) ? nSamples : 0),

		NSOpenGLPFAClosestPolicy,
		0
	};

	this->pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
	this->openGLContext = [[NSOpenGLContext alloc] initWithFormat:this->pixelFormat shareContext:nil];
	[this->openGLContext setView:openGLView];

	[openGLView setPixelFormat:this->pixelFormat];
	[openGLView setOpenGLContext:this->openGLContext];

	this->Init(loader);
}