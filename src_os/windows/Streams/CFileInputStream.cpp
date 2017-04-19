/*
* Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include "../../../headers/Streams/CFileInputStream.h"
//Global
#include <Windows.h>
//Local
#include "../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../headers/ErrorHandling/CFileNotFoundException.hpp"
//Namespaces
using namespace ACStdLib;

//Constructor
CFileInputStream::CFileInputStream(const CPath &refPath)
{
	CUTF16String fileNameUTF16(refPath.GetString().GetUTF16());

	this->hitEnd = false;
	this->pFileHandle = CreateFileW((LPCWSTR)fileNameUTF16.GetC_Str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(this->pFileHandle == INVALID_HANDLE_VALUE)
	{
		switch(GetLastError())
		{
		case ERROR_PATH_NOT_FOUND:
			throw ErrorHandling::CFileNotFoundException(refPath);
		}
	}
}

//Destructor
CFileInputStream::~CFileInputStream()
{
	CloseHandle(this->pFileHandle);
}

//Public methods
uint64 CFileInputStream::GetCurrentOffset() const
{
	LARGE_INTEGER offset, result;

	offset.QuadPart = 0;

	SetFilePointerEx(this->pFileHandle, offset, &result, FILE_CURRENT);

	return result.QuadPart;
}

uint64 CFileInputStream::GetRemainingBytes() const
{
	return this->GetSize() - this->GetCurrentOffset();
}

uint64 CFileInputStream::GetSize() const
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

byte CFileInputStream::ReadByte()
{
	byte b;
	DWORD nReadBytes;

	ReadFile(this->pFileHandle, &b, 1, &nReadBytes, nullptr);
	this->hitEnd = nReadBytes == 0;

	return b;
}

uint32 CFileInputStream::ReadBytes(void *pDestination, uint32 count)
{
	DWORD nReadBytes;

	ReadFile(this->pFileHandle, pDestination, count, &nReadBytes, nullptr);
	this->hitEnd = nReadBytes == 0;

	return nReadBytes;
}

void CFileInputStream::SetCurrentOffset(uint64 offset)
{
	LARGE_INTEGER liOffset;

	liOffset.QuadPart = offset;

	SetFilePointerEx(this->pFileHandle, liOffset, nullptr, FILE_BEGIN);
}

uint32 CFileInputStream::Skip(uint32 nBytes)
{
	uint64 currentOffset;
	LARGE_INTEGER liOffset, newFilePointer;

	currentOffset = this->GetCurrentOffset();

	liOffset.QuadPart = nBytes;

	SetFilePointerEx(this->pFileHandle, liOffset, &newFilePointer, FILE_CURRENT);

	return newFilePointer.QuadPart - currentOffset;
}