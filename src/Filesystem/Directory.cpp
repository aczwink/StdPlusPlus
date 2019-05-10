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
#include <Std++/Filesystem/Directory.hpp>
//Local
#include <Std++/Filesystem/DirectoryWalker.hpp>
#include <Std++/Filesystem/FileSystem.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void Directory::CreateDirectoryTree(const Path &directoryPath)
{
	if(directoryPath.IsAbsolute())
		return this->GetFileSystem()->CreateDirectoryTree(directoryPath);
	if(directoryPath.GetString().IsEmpty())
		return;

	Path subPath;
	String part = directoryPath.SplitOutmostPathPart(subPath);

	if(this->ContainsFile(part))
	{
		NOT_IMPLEMENTED_ERROR; //some kind of exeception must be thrown
	}
	else if(!this->ContainsSubDirectory(part))
		this->CreateSubDirectory(part);

	this->GetSubDirectory(part)->CreateDirectoryTree(subPath);
}

FileSystemNodeType Directory::GetType() const
{
	return FileSystemNodeType::Directory;
}

DirectoryWalkerWrapper Directory::WalkFiles()
{
	return DirectoryWalkerWrapper(AutoPointer<const Directory>(this, false));
}