/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/ContainerFileSystem.hpp>
//Local
#include <Std++/Filesystem/ContainerDirectory.hpp>
//Namespaces
using namespace StdPlusPlus;

ContainerFileSystem::ContainerFileSystem(const FileSystemFormat *format, UniquePointer<SeekableInputStream> &&inputStream) : FileSystem(format),
																															 inputStream(Forward(inputStream)),
																															 root(new ContainerDirectory)
{
}

//Public methods
AutoPointer<Directory> ContainerFileSystem::GetDirectory(const Path &directoryPath)
{
	if(directoryPath.IsRoot())
		return this->root;

	Path leftPath = directoryPath;
	if(leftPath.IsAbsolute()) //skip root slash
		leftPath = leftPath.GetString().SubString(1);
	AutoPointer<Directory> currentDir = this->root;
	while(!leftPath.GetString().IsEmpty())
	{
		Path remaining;
		String name = leftPath.SplitOutmostPathPart(remaining);
		leftPath = remaining;

		currentDir = currentDir->GetSubDirectory(name);
	}

	return currentDir;
}


AutoPointer<Directory> ContainerFileSystem::GetRoot()
{
	return this->root;
}

uint64 ContainerFileSystem::GetSize() const
{
	return this->root->GetSize();
}

//Protected methods
void ContainerFileSystem::AddSourceFile(const Path &filePath, uint64 offset, uint64 size)
{
	Path directoryPath = filePath.GetParent();
	this->CreateDirectoryTree(directoryPath);
	AutoPointer<Directory> dir = this->GetDirectory(directoryPath);
	AutoPointer<ContainerDirectory> containerDirectory = dir.DynamicCast<ContainerDirectory>();

	containerDirectory->AddSourceFile(filePath.GetName(), offset, size);
}