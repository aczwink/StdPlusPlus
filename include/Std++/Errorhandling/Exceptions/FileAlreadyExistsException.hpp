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
#pragma once
//Local
#include <Std++/Errorhandling/Exception.hpp>

namespace StdXX::ErrorHandling
{
	class STDPLUSPLUS_API FileAlreadyExistsException : public Exception
	{
	public:
		//Members
		FileSystem::Path path;

		//Constructor
		inline FileAlreadyExistsException(const FileSystem::Path &path)
		{
			this->path = path;
		}

		//Inline
		inline String Description() const
		{
			return u8"Tried to write file or create directory, but file already exist for path: \"" + this->path.GetString() + u8"\"";
		}
	};
}