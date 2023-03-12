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
//Class header
#include <Std++/FileSystem/Buffered/ArchiveFileSystem.hpp>
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/FileSystem/Buffered/EmbeddedFileInputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Compression/Decompressor.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

/*
//Local
#include <Std++/FileSystem/WritableContainerDirectory.hpp>
#include <Std++/FileSystem/OSFileSystem.hpp>
#include <Std++/FileSystem/DirectoryWalker.hpp>

//Public methods
bool ArchiveFileSystem::Exists(const Path &path) const
{
	return this->root->Exists(path);
}
*/

//Public methods
UniquePointer <InputStream> ArchiveFileSystem::OpenFileForReading(const Path &path, bool verify) const
{
	AutoPointer<const ContainerFileBase> file = this->FindFile(path);
	if(file.IsNull())
		return nullptr;

	const ContainerFileHeader& header = file->Info();

	InputStream* input = new EmbeddedFileInputStream(
			header.offset,
			header.storedSize, //we only read the data block
			this->containerInputStream,
			this->containerInputStreamLock);

	ChainedInputStream* chain = new ChainedInputStream(input);
	if (header.compression.HasValue())
	{
		chain->Add(new BufferedInputStream(chain->GetEnd())); //add a buffer for performance
		chain->Add(Decompressor::Create(*header.compression, chain->GetEnd(), verify));
	}

	this->AddFilters(*chain, header, verify);

	return chain;
}

SpaceInfo StdXX::FileSystem::ArchiveFileSystem::QuerySpace() const
{
	return {
		.freeSize = 0,
		.totalSize = this->containerInputStream.QuerySize()
	};
}

Optional <Path> StdXX::FileSystem::ArchiveFileSystem::ReadLinkTarget(const Path &path) const
{
	NOT_IMPLEMENTED_ERROR;
	return Optional<Path>();
}

//Protected methods
AutoPointer<const MemoryDirectory> ArchiveFileSystem::GetRoot() const
{
	return this->root;
}

//Private methods
void ArchiveFileSystem::AddFilters(ChainedInputStream &chainedInputStream, const ContainerFileHeader& header, bool verify) const
{
}

AutoPointer<MemoryDirectory> ArchiveFileSystem::CreateOrQueryDirectory(const Path& directoryPath)
{
	Path rest;
	String current = directoryPath.SplitOutmostPathPart(rest);
	ASSERT_EQUALS(true, current.IsEmpty());

	if(this->root.IsNull())
		this->root = new MemoryDirectory;

	AutoPointer<MemoryDirectory> dir = this->root;
	while (!rest.String().IsEmpty())
	{
		current = rest.SplitOutmostPathPart(rest);
		AutoPointer<MemoryFileNode> child = dir->GetChild(current);
		if(child.IsNull())
		{
			dir->AddChild(current, new MemoryDirectory);
			dir = dir->GetChild(current).MoveCast<MemoryDirectory>();
		}
		else
			dir = child.MoveCast<MemoryDirectory>();
	}

	return dir.Cast<MemoryDirectory>();
}