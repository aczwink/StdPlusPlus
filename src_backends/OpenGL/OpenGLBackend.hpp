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
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
//Local
#include <Std++/_Backends/RenderBackend.hpp>

namespace StdPlusPlus
{
	typedef void *(*GL_EXT_LOADER)(const char *extensionName);

	class OpenGLBackend : public RenderBackend
	{
	public:
		//Constructor
		OpenGLBackend(GL_EXT_LOADER extensionLoaderFunction);

		//Methods
		void Load() override;

	private:
		//Members
		bool extensionsLoaded;
		GL_EXT_LOADER extensionLoaderFunction;

		//Methods
		void LoadOpenGLExtensions();
	};
}
#endif