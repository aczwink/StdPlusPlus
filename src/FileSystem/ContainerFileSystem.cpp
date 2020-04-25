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
//Class header
#include <Std++/FileSystem/ContainerFileSystem.hpp>
//Local
#include <Std++/FileSystem/ContainerDirectory.hpp>
#include <Std++/FileSystem/OSFileSystem.hpp>
#include <Std++/FileSystem/DirectoryWalker.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
ContainerFileSystem::ContainerFileSystem(const Path &fileSystemPath) : fileSystemPath(fileSystemPath),
																									   containerInputStream((OSFileSystem::GetInstance().Exists(fileSystemPath)) ? (new FileInputStream(fileSystemPath)) : nullptr),
																									   root(new ContainerDirectory(this))
{
	this->isFlushed = !this->containerInputStream.IsNull();
}

//Public methods
void ContainerFileSystem::CreateLink(const Path &linkPath, const Path &linkTargetPath)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

bool ContainerFileSystem::Exists(const Path &path) const
{
	return this->root->Exists(path);
}

AutoPointer<Directory> ContainerFileSystem::GetRoot()
{
	return this->root;
}

AutoPointer<const Directory> ContainerFileSystem::GetRoot() const
{
	return this->root;
}

SpaceInfo ContainerFileSystem::QuerySpace() const
{
	return {
		.availableSize = 0,
		.freeSize = 0,
		.totalSize = this->containerInputStream->QuerySize()
	};
}

//Protected methods
void ContainerFileSystem::AddSourceFile(const Path& filePath, ContainerFile *file)
{
	bool flushStatus = this->isFlushed;

	Path directoryPath = filePath.GetParent();
	this->CreateDirectoryTree(directoryPath);
	AutoPointer<Directory> dir = this->GetDirectory(directoryPath);
	AutoPointer<ContainerDirectory> containerDirectory = dir.Cast<ContainerDirectory>();

	containerDirectory->AddChild(filePath.GetName(), file);

	this->isFlushed = flushStatus; //the flushed status is not affected by source files
}

UniquePointer<FileOutputStream> ContainerFileSystem::OpenTempContainer()
{
	Path p = this->fileSystemPath + Path(u8".stdpptmp");
	ASSERT(!OSFileSystem::GetInstance().Exists(p), u8"If you see this, report to StdXX please");

	return new FileOutputStream(p);
}

void ContainerFileSystem::SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer)
{
	if(!this->containerInputStream.IsNull())
		this->containerInputStream = nullptr; //close file

	Path tempPath = tempContainer->GetPath();
	tempContainer = nullptr; //close temp file

	OSFileSystem::GetInstance().Move(tempPath, this->fileSystemPath);

	//TODO: we now have to reopen this->containerInputStream and read again all offsets...
	//TODO: also all files that have buffers must be closed now... i.e. the buffers must be deleted
}

void ContainerFileSystem::Move(const Path &from, const Path &to)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}