/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <sys/stat.h>
//Local
#include <Std++/Streams/FileInputStream.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
uint64 POSIXFile::GetSize() const
{
	struct stat sb{};
	int ret = stat(reinterpret_cast<const char *>(this->path.GetString().ToUTF8().GetRawZeroTerminatedData()), &sb);
	ASSERT(ret == 0, u8"REPORT THIS PLEASE!");

	return static_cast<uint64>(sb.st_size);
}

uint64 POSIXFile::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
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