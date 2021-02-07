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
#include <Std++/FileSystem/WritableContainerDirectory.hpp>
//Local
#include <Std++/FileSystem/DirectoryIterator.hpp>
#include <Std++/FileSystem/WritableContainerFileSystem.hpp>
#include <Std++/FileSystem/MemoryFile.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
UniquePointer<OutputStream> WritableContainerDirectory::CreateFile(const String &name)
{
	MemoryFile* file = new MemoryFile;
	this->AddChild(name, file);

	this->fileSystem->isFlushed = false;

	return file->OpenForWriting();
}

void WritableContainerDirectory::CreateSubDirectory(const String &name, const Permissions* permissions)
{
	this->AddChild(name, new WritableContainerDirectory(this->fileSystem));

	this->fileSystem->isFlushed = false;
}*/