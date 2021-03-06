/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include <Std++/Streams/FileInputStream.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
#include <Std++/ErrorHandling/FileNotFoundException.hpp>
#include <Std++/Filesystem/OSFileSystem.hpp>
//Namespaces
using namespace StdXX;

//Constructor
FileInputStream::FileInputStream(const Path &path)
{
	this->hitEnd = false;

	String nativePath = OSFileSystem::GetInstance().ToNativePath(path);
	this->pFileHandle = CreateFileW((LPCWSTR)nativePath.GetRawZeroTerminatedData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(this->pFileHandle == INVALID_HANDLE_VALUE)
	{
		DWORD errorCode = GetLastError();
		switch(errorCode)
		{
		case ERROR_PATH_NOT_FOUND:
			throw ErrorHandling::FileNotFoundException(path);
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
}

//Destructor
FileInputStream::~FileInputStream()
{
	CloseHandle(this->pFileHandle);
}

//Public methods
uint64 FileInputStream::GetCurrentOffset() const
{
	LARGE_INTEGER offset, result;

	offset.QuadPart = 0;

	SetFilePointerEx(this->pFileHandle, offset, &result, FILE_CURRENT);

	return result.QuadPart;
}

uint64 FileInputStream::GetRemainingBytes() const
{
	return this->GetSize() - this->GetCurrentOffset();
}

uint64 FileInputStream::GetSize() const
{
	uint64 offset;
	LARGE_INTEGER moveOffset, size;

	moveOffset.QuadPart = 0;

	offset = this->GetCurrentOffset();
	SetFilePointerEx(this->pFileHandle, moveOffset, &size, FILE_END);

	//restore old pos
	moveOffset.QuadPart = offset;
	SetFilePointerEx(this->pFileHandle, moveOffset, nullptr, FILE_BEGIN);

	return size.QuadPart;
}

uint32 FileInputStream::ReadBytes(void *pDestination, uint32 count)
{
	DWORD nReadBytes;

	ReadFile(this->pFileHandle, pDestination, count, &nReadBytes, nullptr);
	this->hitEnd = nReadBytes == 0;

	return nReadBytes;
}

void FileInputStream::SetCurrentOffset(uint64 offset)
{
	LARGE_INTEGER liOffset;

	liOffset.QuadPart = offset;

	SetFilePointerEx(this->pFileHandle, liOffset, nullptr, FILE_BEGIN);
}

uint32 FileInputStream::Skip(uint32 nBytes)
{
	uint64 currentOffset;
	LARGE_INTEGER liOffset, newFilePointer;

	currentOffset = this->GetCurrentOffset();

	liOffset.QuadPart = nBytes;

	SetFilePointerEx(this->pFileHandle, liOffset, &newFilePointer, FILE_CURRENT);

	return newFilePointer.QuadPart - currentOffset;
}