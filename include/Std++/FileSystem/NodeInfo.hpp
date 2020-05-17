/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include "NodePermissions.hpp"

namespace StdXX
{
	struct NodeInfo
	{
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

		UniquePointer<FileSystem::NodePermissions> permissions;
	};
}