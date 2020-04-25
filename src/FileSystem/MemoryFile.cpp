/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/MemoryFile.hpp>
//Namespaces
using namespace StdXX::FileSystem;

//Public methods
void MemoryFile::ChangePermissions(const StdXX::FileSystem::NodePermissions &newPermissions)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

StdXX::UniquePointer<StdXX::InputStream> MemoryFile::OpenForReading(bool verify) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<InputStream>();
}

StdXX::UniquePointer<StdXX::OutputStream> MemoryFile::OpenForWriting()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<OutputStream>();
}

StdXX::NodeInfo MemoryFile::QueryInfo() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return NodeInfo();
}
