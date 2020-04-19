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
#include "POSIXFile.hpp"
//Local
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/FileSystem/UnixPermissions.hpp>
#include "PosixStat.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
uint64 POSIXFile::GetSize() const
{
	uint64 fileSize;
	StatQueryFileInfo(this->path, fileSize);
	return fileSize;
}

UniquePointer<InputStream> POSIXFile::OpenForReading(bool verify) const
{
	return new FileInputStream(this->path);
}

UniquePointer<OutputStream> POSIXFile::OpenForWriting()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}