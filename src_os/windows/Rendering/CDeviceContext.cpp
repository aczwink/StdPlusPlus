/*
* Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Windows.h>
//Local
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Rendering;

//Private methods
/*
void DeviceContext::CreateOSContext(const RenderTargetWidget &renderTargetWidget, uint8 nSamples)
{
	this->screenFrameBufferId = 0; //on windows we have the real screen frame buffer
}*/

/*
void DeviceContext::UnbindOSContext()
{
	NOT_IMPLEMENTED_ERROR; //TODO: does not belong here
	wglMakeCurrent(nullptr, nullptr);
	g_hCurrentGLCtx = nullptr;
}*/