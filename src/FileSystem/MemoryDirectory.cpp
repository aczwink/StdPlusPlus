/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/MemoryDirectory.hpp>
//Local
#include <Std++/_Backends/DirectoryIteratorState.hpp>
#include <Std++/FileSystem/DirectoryIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
void MemoryDirectory::ChangePermissions(const Permissions &newPermissions)
{
	this->permissions = newPermissions.Clone();
}

UniquePointer<OutputStream> MemoryDirectory::CreateFile(const String &name)
{
	NOT_IMPLEMENTED_ERROR;
	return UniquePointer<OutputStream>();
}

void MemoryDirectory::CreateSubDirectory(const String &name, const Permissions *permissions)
{
	NOT_IMPLEMENTED_ERROR;
}

bool MemoryDirectory::Exists(const Path &path) const
{
	Path leftPart;
	Path child = path.SplitOutmostPathPart(leftPart);
	const String &childString = child.String();
	if(leftPart.String().IsEmpty())
		return this->children.Contains(childString);

	if(this->HasSubDirectory(childString))
		return this->children[childString].Cast<Directory>()->Exists(leftPart);
	return false;
}

AutoPointer<Node> MemoryDirectory::GetChild(const String &name)
{
	if(!this->children.Contains(name))
		return nullptr;
	return this->children[name];
}

bool MemoryDirectory::IsEmpty() const
{
	return this->children.IsEmpty();
}
*/