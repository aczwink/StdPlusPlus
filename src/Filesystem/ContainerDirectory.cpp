/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Filesystem/ContainerDirectory.hpp>
//Local
#include <ACStdLib/Filesystem/DirectoryIterator.hpp>
//Namespaces
using namespace ACStdLib;

//Public methods
bool ContainerDirectory::ContainsSubDirectory(const String &name) const
{
	return this->subDirectories.Contains(name);
}

void ContainerDirectory::CreateSubDirectory(const String &name)
{
	this->subDirectories[name] = new ContainerDirectory;
}

uint64 ContainerDirectory::GetSize() const
{
	uint64 sum = 0;
	for(const auto &kv : this->files)
		sum += kv.value->GetSize();
	for(const auto &kv : this->subDirectories)
		sum += kv.value->GetSize();

	return sum;
}

AutoPointer<Directory> ContainerDirectory::GetSubDirectory(const String &name)
{
	return this->subDirectories[name].StaticCast<Directory>();
}

//For range-based loop
DirectoryIterator ContainerDirectory::begin() const
{
	NOT_IMPLEMENTED_ERROR;
}

DirectoryIterator ContainerDirectory::end() const
{
	NOT_IMPLEMENTED_ERROR;
}

//Private methods
void ContainerDirectory::AddSourceFile(String fileName, uint64 offset, uint64 size)
{
	ContainerFile *file = new ContainerFile();
	file->offset = offset;
	file->size = size;
	this->files[fileName] = file;
}