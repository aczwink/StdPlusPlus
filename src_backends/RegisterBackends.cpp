/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/_Backends/BackendManager.hpp>
//Backends
#include "gtk/GTKBackend.hpp"
//Namespaces
using namespace ACStdLib;

#define PRIORITY_HIGH 0

#define ADD_BACKEND(backend, priority) this->backends[(backend)->GetType()].Insert(priority, backend);

//Private methods
void BackendManager::RegisterBackends()
{
#ifdef _ACSTDLIB_BACKEND_GTK3
	GTKBackend *gtkBackend = new GTKBackend;

	ADD_BACKEND(gtkBackend, PRIORITY_HIGH);
#endif
}