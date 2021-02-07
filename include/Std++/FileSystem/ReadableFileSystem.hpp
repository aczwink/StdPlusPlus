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
#include <Std++/Optional.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Path.hpp"
#include "FileInfo.hpp"
#include "DirectoryEnumerator.hpp"

namespace StdXX::FileSystem
{
	struct SpaceInfo
	{
		uint64 freeSize;
		uint64 totalSize;
	};

	class ReadableFileSystem
	{
	public:
		//Destructor
		virtual ~ReadableFileSystem() = default;

		//Abstract
		virtual UniquePointer<DirectoryEnumerator> EnumerateChildren(const Path& path) const = 0;
		virtual UniquePointer<InputStream> OpenFileForReading(const Path& path, bool verify) const = 0;
		virtual Optional<FileInfo> QueryFileInfo(const Path& path) const = 0;
		virtual SpaceInfo QuerySpace() const = 0;
		virtual Optional<Path> ReadLinkTarget(const Path& path) const = 0;
	};
}