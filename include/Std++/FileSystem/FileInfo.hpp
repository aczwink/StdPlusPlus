/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Time/DateTime.hpp>
#include <Std++/Optional.hpp>
#include "Permissions.hpp"

namespace StdXX::FileSystem
{
	enum class FileType
	{
		Directory,
		File,
		Link
	};

	struct FileInfo
	{
		FileType type;

		/**
		 * The size of the content of the file/link/directory.
		 */
		uint64 size;

		/**
		 * The last modified time is not stored by all filesystems.
		 * Also not all operating system can guarantee a precision smaller than seconds (or not even seconds).
		 */
		Optional<DateTime> lastModifiedTime;

		/**
		 * The number of bytes that the node occupies on the file system.
		 * For example a file that is compressed on the filesystem may have a less stored size than its actual size.
		 */
		uint64 storedSize;

		UniquePointer<Permissions> permissions;

		//Constructors
		FileInfo() = default;

		inline FileInfo(FileType fileType)
		{
			this->type = fileType;
		}

		inline FileInfo(const FileInfo& other)
		{
			*this = other;
		}

		FileInfo(FileInfo&&) = default;

		//Operators
		FileInfo& operator=(const FileInfo& other)
		{
			this->type = other.type;
			this->size = other.size;
			this->lastModifiedTime = other.lastModifiedTime;
			this->storedSize = other.storedSize;
			this->permissions = other.permissions.IsNull() ? nullptr : other.permissions->Clone();

			return *this;
		}

		FileInfo& operator=(FileInfo&& other) = default;
	};
}