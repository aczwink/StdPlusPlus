/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/Streams/ChainedInputStream.hpp>
#include "BufferedMetadataFileSystem.hpp"
#include "ContainerFile.hpp"

namespace StdXX::FileSystem
{
	/**
	 * An archive file system is usually a filesystem stored inside one file (i.e. a zip-file etc.).
	 * These containers are actually not designed for an in-use filesystem. They are written once and are then used,
	 * only for data retrieval (read only).
	 * A typical layout of a container is to write all file headers first and then the data of all files.
	 * However, when you now want to add a file to the container, you have to create the container again, because you
	 * need to change the file headers, though you can not append to them because there is already file data there.
	 */
	class ArchiveFileSystem : public BufferedMetadataFileSystem
	{
	public:
		//Constructor
		inline ArchiveFileSystem(SeekableInputStream& containerInputStream) : containerInputStream(containerInputStream)
		{
		}

		//Methods
		UniquePointer <InputStream> OpenFileForReading(const Path &path, bool verify) const override;
		SpaceInfo QuerySpace() const override;
		Optional <Path> ReadLinkTarget(const Path &path) const override;

	protected:
		//Methods
		const MemoryDirectory* GetRoot() const override;

		//Inline
		inline void AddSourceDirectory(const Path& nodePath, FileInfo&& fileInfo)
		{
			ASSERT_EQUALS(FileType::Directory, fileInfo.type);
			this->AddNode(nodePath, new MemoryDirectory(Move(fileInfo)));
		}

		inline void AddSourceFile(const Path& nodePath, ContainerFileHeader&& header)
		{
			ASSERT_EQUALS(FileType::File, header.type);
			this->AddNode(nodePath, new ContainerFile<ContainerFileHeader>(Move(header)));
		}

		inline void AddSourceFile(const Path& nodePath, ContainerFileBase* file)
		{
			this->AddNode(nodePath, file);
		}

	private:
		//Members
		UniquePointer<MemoryDirectory> root;
		mutable Mutex containerInputStreamLock;
		SeekableInputStream& containerInputStream;

		//Methods
		virtual void AddFilters(ChainedInputStream& chainedInputStream, const ContainerFileHeader& header, bool verify) const;
		MemoryDirectory* CreateOrQueryDirectory(const Path& directoryPath);

		//Inline
		inline void AddNode(const Path& nodePath, FileMetadataNode* node)
		{
			MemoryDirectory* dir = this->CreateOrQueryDirectory(nodePath.GetParent());
			dir->SetChild(nodePath.GetName(), node);
		}

		inline const ContainerFileBase* FindFile(const Path& path) const
		{
			const FileMetadataNode* node = this->FindNode(path);
			ASSERT(node != nullptr, u8"Node does not exist.");
			ASSERT(IS_INSTANCE_OF(node, ContainerFileBase), u8"Node is not a file.");

			return dynamic_cast<const ContainerFileBase *>(node);
		}
	};

	template<typename T>
	class CustomArchiveFileSystem : public ArchiveFileSystem
	{
	public:
		//Constructor
		inline CustomArchiveFileSystem(SeekableInputStream &containerInputStream) : ArchiveFileSystem(containerInputStream)
		{
		}

	protected:
		//Inline
		inline void AddSourceFile(const Path& nodePath, const T& fileInfo)
		{
			ASSERT_EQUALS(FileType::File, fileInfo.type);
			ArchiveFileSystem::AddSourceFile(nodePath, new ContainerFile<T>(fileInfo));
		}

	private:
		//Abstract
		virtual void AddTypedFilters(ChainedInputStream &chainedInputStream, const T& header, bool verify) const = 0;

		//Methods
		void AddFilters(ChainedInputStream &chainedInputStream, const ContainerFileHeader &header, bool verify) const override
		{
			this->AddTypedFilters(chainedInputStream, (const T&)header, verify);
		}
	};
}