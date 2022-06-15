/*
* Copyright (c) 2018-2022 Amir Czwink (amir130@hotmail.de)
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
#include "7zip_FileSystemFormat.hpp"
//Local
#include <Std++/FileTypes/UTIConstants.hpp>
#include "7zip_FileSystem.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
RWFileSystem* SevenZip_FileSystemFormat::CreateFileSystem(const Path & fileSystemPath) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

String SevenZip_FileSystemFormat::GetId() const
{
	return FileTypes::UTI::_7zip;
}

String SevenZip_FileSystemFormat::GetName() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return String();
}

float32 SevenZip_FileSystemFormat::Matches(SeekableInputStream & inputStream) const
{
	const byte signature[6] = { u8'7', u8'z', 0xbc, 0xaf, 0x27, 0x1c };

	byte readSignature[6];
	inputStream.ReadBytes(readSignature, sizeof(readSignature));

	if (MemCmp(signature, readSignature, sizeof(signature)) == 0)
		return 1;
	return 0;
}

RWFileSystem *SevenZip_FileSystemFormat::OpenFileSystem(const Path &fileSystemPath, const OpenOptions& options) const
{
	return nullptr;
}

ReadableFileSystem *SevenZip_FileSystemFormat::OpenFileSystemReadOnly(const Path &fileSystemPath, const OpenOptions& options) const
{
	NOT_IMPLEMENTED_ERROR;
	//return new SevenZip_FileSystem(fileSystemPath);
}*/