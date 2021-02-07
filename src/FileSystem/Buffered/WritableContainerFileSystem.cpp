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
/*//Class header
#include <Std++/FileSystem/WritableContainerFileSystem.hpp>
//Local
#include <Std++/FileSystem/OSFileSystem.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
WritableContainerFileSystem::WritableContainerFileSystem(const Path &fileSystemPath)
	: fileInputStream((OSFileSystem::Instance().Exists(fileSystemPath)) ? (new FileInputStream(fileSystemPath)) : nullptr),
	ArchiveFileSystem(*fileInputStream), fileSystemPath(fileSystemPath)
{
	this->isFlushed = !this->fileInputStream.IsNull();
}

//Public methods
void WritableContainerFileSystem::CreateLink(const Path &linkPath, const Path &linkTargetPath)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

AutoPointer<Directory> WritableContainerFileSystem::GetRoot()
{
	return this->root;
}

//Protected methods
void WritableContainerFileSystem::Move(const Path &from, const Path &to)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

UniquePointer<FileOutputStream> WritableContainerFileSystem::OpenTempContainer()
{
	Path p = this->fileSystemPath + Path(u8".stdpptmp");
	ASSERT(!OSFileSystem::Instance().Exists(p), u8"If you see this, report to StdXX please");

	return new FileOutputStream(p);
}

void WritableContainerFileSystem::SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer)
{
	if(!this->fileInputStream.IsNull())
		this->fileInputStream = nullptr; //close file

	Path tempPath = tempContainer->GetPath();
	tempContainer = nullptr; //close temp file

	OSFileSystem::Instance().Move(tempPath, this->fileSystemPath);

	//TODO: we now have to reopen this->containerInputStream and read again all offsets...
	//TODO: also all files that have buffers must be closed now... i.e. the buffers must be deleted
}*/