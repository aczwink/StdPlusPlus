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
#include <Std++/Streams/FileOutputStream.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
//Namespaces
using namespace StdXX;

//Constructor
FileOutputStream::FileOutputStream(const Path &refPath, bool overwrite)
{
	this->pFileHandle = CreateFileW((LPCWSTR)refPath.GetString().ToUTF16().GetRawZeroTerminatedData(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, overwrite ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
}

//Destructor
FileOutputStream::~FileOutputStream()
{
	CloseHandle(this->pFileHandle);
}

//Public methods
uint64 FileOutputStream::GetCurrentOffset() const
{
	LARGE_INTEGER offset, result;

	offset.QuadPart = 0;

	SetFilePointerEx(this->pFileHandle, offset, &result, FILE_CURRENT);

	return result.QuadPart;
}

void FileOutputStream::SetCurrentOffset(uint64 offset)
{
	LARGE_INTEGER liOffset;

	liOffset.QuadPart = offset;

	SetFilePointerEx(this->pFileHandle, liOffset, nullptr, FILE_BEGIN);
}

uint32 FileOutputStream::WriteBytes(const void *pSource, uint32 size)
{
	DWORD nBytesWritten;

	WriteFile(this->pFileHandle, pSource, size, &nBytesWritten, NULL);

	return nBytesWritten;
}