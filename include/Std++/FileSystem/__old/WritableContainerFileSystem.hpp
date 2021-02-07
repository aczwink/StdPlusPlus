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
//Local
#include "Path.hpp"
#include "ContainerFileSystem.hpp"
#include "WritableContainerDirectory.hpp"
#include "BufferedWritableMetadataFileSystem.hpp"

namespace StdXX::FileSystem
{
	/**
	 * This class manages exactly these containers by using the container file for reading files and when files are
	 * written to this filesystem, they are buffered in memory and are not flushed to the container.
	 * As soon as you issue the Flush call, this class will create a temporary file, copy the current filesystem to
	 * the new container (i.e. copy old still available files from this filesystem and then add the memory files,
	 * that were written to this filesystem) and then exchanges the two container files.
	 * On successful exchange, the old filesystem is dropped.
	 *
	 * It's important to know these facts, when implementing a ArchiveFileSystem. The implications are the following:
	 * -All files you read, are read from the original container
	 * -All files you write are written into memory! (This is especially important if a system has no kind of swap storage)
	 * -The container never gets corrupted. If your application crashes, the container wasn't modified and is thus still
	 * stable. Only when you flush, a new container is created and the old one is only deleted if exchanging the
	 * containers was successful.
	 */
	class WritableContainerFileSystem : public ArchiveFileSystem, public BufferedWritableMetadataFileSystem
	{
		friend class WritableContainerDirectory;
	public:
		//Constructor
		WritableContainerFileSystem(const Path &fileSystemPath);

		//Methods
		void CreateLink(const Path &linkPath, const Path &linkTargetPath) override;
		AutoPointer<Directory> GetRoot() override;
		void Move(const Path &from, const Path &to) override;

	protected:
		//Members
		bool isFlushed;
		Path fileSystemPath;

		//Methods
		UniquePointer<FileOutputStream> OpenTempContainer();
		void SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer);

	private:
		//Members
		UniquePointer<FileInputStream> fileInputStream;
	};
}