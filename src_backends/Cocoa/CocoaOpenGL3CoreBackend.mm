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
#include "CocoaOpenGL3CoreBackend.hh"
//Local
#import "Rendering/CocoaOpenGL3CoreDeviceContext.hh"
#import "UI/CocoaWindowBackend.hh"
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;

//Public methods
DeviceContext *CocoaOpenGL3CoreBackend::CreateDeviceContext(const WindowBackend &backend, uint8 nSamples) const
{
	CocoaWindowBackend &cocoaWindowBackend = (CocoaWindowBackend &) backend;
	NSOpenGLView *openGLView = (NSOpenGLView *)cocoaWindowBackend.GetView();

	return new CocoaOpenGL3CoreDeviceContext(openGLView, nSamples);
}
