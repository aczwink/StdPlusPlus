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
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
#ifdef _STDPLUSPLUS_BACKEND_GTK3
//Local
#include "../OpenGL3Core/OpenGL3CoreBackend.hpp"

namespace StdXX
{
	class Gtk3OpenGLBackend : public OpenGL3CoreBackend
	{
	public:
		//Methods
		Rendering::DeviceContext *CreateDeviceContext(_stdxx_::WidgetBackend &backend) const override;
	};
}
#endif
#endif