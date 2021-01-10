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
//Class header
#include "ZipFile.hpp"
//Local
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/Streams/CheckedInputStream.hpp>
#include "Zip.hpp"
#include "ZipFileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void ZipFile::ChangePermissions(const FileSystem::NodePermissions &newPermissions)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

UniquePointer<InputStream> ZipFile::OpenForReading(bool verify) const
{
	return ZipReadableFile::OpenForReading(verify);
}

StdXX::UniquePointer<StdXX::OutputStream> _stdxx_::ZipFile::OpenForWriting() {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::UniquePointer<StdXX::OutputStream>();
}

NodeInfo ZipFile::QueryInfo() const
{
	//TODO: last mod time and permissions
	return {
		.size = this->fileHeader.uncompressedSize,
		.storedSize = this->fileHeader.compressedSize,
	};
}