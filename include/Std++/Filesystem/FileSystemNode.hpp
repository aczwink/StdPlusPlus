/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "../Containers/Strings/String.hpp"
#include "../SmartPointers/AutoPointer.hpp"
#include "Path.hpp"

namespace StdXX
{
	//Forward declarations
	class Directory;
	class FileSystem;

	class FileSystemNode
	{
	public:
		//Destructor
		virtual ~FileSystemNode(){};

		//Abstract
		/**
		 * Returns the number of bytes that the node occupies on the file system.
		 * For example a file that is compressed on the filesystem may have a less stored size than its actual size.
		 * @return
		 */
		virtual uint64 GetStoredSize() const = 0;

		/**
		 * For a file: Returns the size of the file in bytes.
		 * Else: 0
		 * @return
		 */
		virtual uint64 GetSize() const = 0;
	};
}