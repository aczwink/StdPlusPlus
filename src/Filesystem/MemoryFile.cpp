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
#include <Std++/Filesystem/MemoryFile.hpp>

//Public methods
void StdXX::MemoryFile::ChangePermissions(const StdXX::Filesystem::NodePermissions &newPermissions)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint64 StdXX::MemoryFile::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

StdXX::UniquePointer<StdXX::InputStream> StdXX::MemoryFile::OpenForReading(bool verify) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<InputStream>();
}

StdXX::UniquePointer<StdXX::OutputStream> StdXX::MemoryFile::OpenForWriting()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<OutputStream>();
}

StdXX::FileSystemNodeInfo StdXX::MemoryFile::QueryInfo() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return FileSystemNodeInfo();
}
