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
//Class header
#include "Gtk3OpenGLBackend.hpp"
//Local
#include "Rendering/GtkOpenGLDeviceContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
Rendering::DeviceContext *Gtk3OpenGLBackend::CreateDeviceContext(WidgetBackend &backend) const
{
	Gtk3WidgetBackend& gtkWindowBackend = dynamic_cast<Gtk3WidgetBackend &>(backend);
	return new GtkOpenGLDeviceContext(gtkWindowBackend, this->LoadWindowSystemOpenGLExtension);
}