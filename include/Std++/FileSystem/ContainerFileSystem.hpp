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
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include "../Streams/SeekableInputStream.hpp"
#include "ContainerFile.hpp"
#include "BufferedMetadataFileSystem.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A container file system is usually a filesystem stored inside one file (i.e. a zip-file etc.).
	 * These containers are actually not designed for an in-use filesystem. They are written once and are then used,
	 * only for data retrieval (read only).
	 * A typical layout of a container is to write all file headers first and then the data of all files.
	 * However, when you now want to add a file to the container, you have to create the container again, because you
	 * need to change the file headers, however, you can not append to them because there is already file data there.
	 *
	 * This class manages exactly these containers by using the container file for reading files and when files are
	 * written to this filesystem, they are buffered in memory and are not flushed to the container.
	 * As soon as you issue the Flush call, this class will create a temporary file, copy the current filesystem to
	 * the new container (i.e. copy old still available files from this filesystem and then add the memory files,
	 * that were written to this filesystem) and then exchanges the two container files.
	 * On successful exchange, the old filesystem is dropped.
	 *
	 * It's important to know these facts, when implementing a ContainerFileSystem. The implications are the following:
	 * -All files you read, are read from the original container
	 * -All files you write are written into memory! (This is especially important if a system has no kind of swap storage)
	 * -The container never gets corrupted. If your application crashes, the container wasn't modified and is thus still
	 * stable. Only when you flush, a new container is created and the old one is only deleted if exchanging the
	 * containers was successful.
	 *
	 */
	class STDPLUSPLUS_API ContainerFileSystem : public BufferedMetadataFileSystem
	{
		friend class ContainerDirectory;
		friend class ContainerFile;
	public:
		//Constructor
		ContainerFileSystem(const Path &fileSystemPath);

		//Methods
		void CreateLink(const Path &linkPath, const Path &linkTargetPath) override;
		bool Exists(const Path &path) const override;
		AutoPointer<Directory> GetRoot() override;
		AutoPointer<const Directory> GetRoot() const override;
		void Move(const Path &from, const Path &to) override;
		SpaceInfo QuerySpace() const override;

	protected:
		//Members
		AutoPointer<Directory> root;
		Path fileSystemPath;
		Mutex containerInputStreamLock;
		UniquePointer<FileInputStream> containerInputStream;
		bool isFlushed;

		//Methods
		void AddSourceFile(const Path& filePath, ContainerFile* file);
		UniquePointer<FileOutputStream> OpenTempContainer();
		void SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer);

		//Inline
		inline void AddSourceFile(const Path& filePath, const ContainerFileHeader& header)
		{
			ContainerFile *file = new ContainerFile(header, this);
			this->AddSourceFile(filePath, file);
		}
	};
}