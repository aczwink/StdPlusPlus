/*
* Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/FileSystemsManager.hpp>
#include <Std++/Filesystem/OSFileSystem.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructor
FileOutputStream::FileOutputStream(const Path& path, bool overwrite, const Permissions* permissions)
{
	String nativePath = FileSystemsManager::Instance().OSFileSystem().ToNativePath(path);
	OutputDebugStringW((LPCWSTR)nativePath.ToUTF16().GetRawZeroTerminatedData());
	this->pFileHandle = CreateFileW((LPCWSTR)nativePath.ToUTF16().GetRawZeroTerminatedData(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, overwrite ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (this->pFileHandle == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		switch (dwErr)
		{
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
}

//Destructor
FileOutputStream::~FileOutputStream()
{
	CloseHandle(this->pFileHandle);
}

//Public methods
void FileOutputStream::Flush()
{
	FlushFileBuffers(this->pFileHandle);
}

uint64 FileOutputStream::QueryCurrentOffset() const
{
	LARGE_INTEGER offset, result;

	offset.QuadPart = 0;

	SetFilePointerEx(this->pFileHandle, offset, &result, FILE_CURRENT);

	return result.QuadPart;
}

void FileOutputStream::SeekTo(uint64 offset)
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