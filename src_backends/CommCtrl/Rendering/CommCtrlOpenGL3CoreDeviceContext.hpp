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
#include "WGL.h" //this must be included before gl headers!!!
#include "../../OpenGL3Core/Rendering/OpenGLDeviceContext.hpp"

namespace _stdxx_
{
	class CommCtrlOpenGL3CoreDeviceContext : public OpenGLDeviceContext
	{
	public:
		//Constructor
		CommCtrlOpenGL3CoreDeviceContext(HDC hDC, uint8 nSamples, GL_EXT_LOADER loader);

		//Destructor
		~CommCtrlOpenGL3CoreDeviceContext();

		//Methods
		void SwapBuffers() override;

	protected:
		//Methods
		void MakeContextCurrent() const override;

	private:
		//Members
		HDC hDC;
		HGLRC hGLRC;
	};
}