/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include "Std++/Streams/SeekableInputStream.hpp"
#include "ContainerFile.hpp"
#include "BufferedMetadataFileSystem.hpp"
#include "MemoryDirectory.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A container file system is usually a filesystem stored inside one file (i.e. a zip-file etc.).
	 * These containers are actually not designed for an in-use filesystem. They are written once and are then used,
	 * only for data retrieval (read only).
	 * A typical layout of a container is to write all file headers first and then the data of all files.
	 * However, when you now want to add a file to the container, you have to create the container again, because you
	 * need to change the file headers, though you can not append to them because there is already file data there.
	 */
	class STDPLUSPLUS_API ArchiveFileSystem : public BufferedMetadataFileSystem
	{
	public:
		//Methods
		bool Exists(const Path &path) const override;
		AutoPointer<const Directory> GetRoot() const override;
		SpaceInfo QuerySpace() const override;
	};
}