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
#include <Std++/FileSystem/Directory.hpp>
//Local
#include <Std++/FileSystem/DirectoryWalker.hpp>
#include <Std++/FileSystem/RWFileSystem.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
void Directory::CreateDirectoryTree(const Path &directoryPath)
{
	ASSERT(directoryPath.IsRelative(), u8"Directory can't move upwards");
	if(directoryPath.String().IsEmpty())
		return;

	Path subPath;
	String part = directoryPath.SplitOutmostPathPart(subPath);

	if(this->HasFile(part))
	{
		NOT_IMPLEMENTED_ERROR; //some kind of exeception must be thrown
	}
	else if(!this->HasSubDirectory(part))
		this->CreateSubDirectory(part);

	this->GetSubDirectory(part)->CreateDirectoryTree(subPath);
}

NodeType Directory::GetType() const
{
	return NodeType::Directory;
}

DirectoryWalkerWrapper Directory::WalkFiles()
{
	return DirectoryWalkerWrapper(AutoPointer<const Directory>(this, false));
}