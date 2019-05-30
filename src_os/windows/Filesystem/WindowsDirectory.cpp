/*
* Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "WindowsDirectory.hpp"
//Global
#include <Windows.h>
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
#include <Std++/Filesystem/OSFileSystem.hpp>
#include "WindowsFile.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
//Definitions
#undef CreateFile

class WindowsDirectoryIteratorState : public DirectoryIteratorState
{
public:
	//Constructor
	inline WindowsDirectoryIteratorState(const Path& path) : path(path)
	{
		String nativePath = OSFileSystem::GetInstance().ToNativePath(path / String(u8"*"));
		this->handle = FindFirstFileW((LPCWSTR)nativePath.ToUTF16().GetRawZeroTerminatedData(), &this->wfd);
	}

	//Destructor
	~WindowsDirectoryIteratorState()
	{
		if(this->handle)
			FindClose(this->handle);
	}

	//Methods
	bool Equals(DirectoryIteratorState * other) const override
	{
		if ((other == nullptr) && (this->handle == nullptr))
			return true;
		if (IS_INSTANCE_OF(other, WindowsDirectoryIteratorState))
		{
			WindowsDirectoryIteratorState* otherTyped = dynamic_cast<WindowsDirectoryIteratorState*>(other);
			return otherTyped->handle == this->handle;
		}
		return false;
	}

	Tuple<String, AutoPointer<FileSystemNode>> GetCurrent() override
	{
		String nodeName = String::CopyRawString((uint16*)wfd.cFileName);

		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			return { nodeName, new WindowsDirectory(this->path / nodeName) };
		return { nodeName, new WindowsFile(this->path / nodeName) };
	}

	void Next() override
	{
		if (!FindNextFileW(this->handle, &this->wfd))
		{
			DWORD error = GetLastError();
			switch (error)
			{
			case ERROR_NO_MORE_FILES:
			{
				FindClose(this->handle);
				this->handle = nullptr;
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR;
			}
		}
	}

private:
	//Members
	const Path& path;
	HANDLE handle;
	WIN32_FIND_DATAW wfd;
};

//Public methods
bool WindowsDirectory::ContainsFile(const String & name) const
{
	String nativePath = OSFileSystem::GetInstance().ToNativePath((this->path / name).GetString());
	DWORD attribs = GetFileAttributesW((LPCWSTR)nativePath.GetRawZeroTerminatedData());
	if (attribs == INVALID_FILE_ATTRIBUTES)
		return false;
	if (attribs & FILE_ATTRIBUTE_DIRECTORY)
		return false;
	return true;
}

bool WindowsDirectory::ContainsSubDirectory(const String & name) const
{
	String nativePath = OSFileSystem::GetInstance().ToNativePath((this->path / name).GetString());
	DWORD attribs = GetFileAttributesW((LPCWSTR)nativePath.GetRawZeroTerminatedData());
	if (attribs == INVALID_FILE_ATTRIBUTES)
		return false;
	if (attribs & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

void WindowsDirectory::CreateSubDirectory(const String & name)
{
	String nativePath = OSFileSystem::GetInstance().ToNativePath((this->path / name).GetString());
	OutputDebugStringW((LPCWSTR)nativePath.GetRawZeroTerminatedData());
	BOOL result = CreateDirectoryW((LPCWSTR)nativePath.GetRawZeroTerminatedData(), NULL);
	if (result == 0)
	{
		DWORD error = GetLastError();
		switch (error)
		{
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
}

DirectoryIterator WindowsDirectory::begin() const
{
	return DirectoryIterator(new WindowsDirectoryIteratorState(this->path));
}

DirectoryIterator WindowsDirectory::end() const
{
	return DirectoryIterator(nullptr);
}





//NOT IMPLEMENTED
UniquePointer<OutputStream> _stdxx_::WindowsDirectory::CreateFile(const String & name)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<OutputStream>();
}

bool _stdxx_::WindowsDirectory::Exists(const Path & path) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

AutoPointer<const File> _stdxx_::WindowsDirectory::GetFile(const String & name) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::AutoPointer<const File>();
}

FileSystem * _stdxx_::WindowsDirectory::GetFileSystem()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

const FileSystem * _stdxx_::WindowsDirectory::GetFileSystem() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

AutoPointer<const Directory> _stdxx_::WindowsDirectory::GetParent() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::AutoPointer<const Directory>();
}

Path _stdxx_::WindowsDirectory::GetPath() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return Path();
}

AutoPointer<Directory> _stdxx_::WindowsDirectory::GetSubDirectory(const String & name)
{
	return new WindowsDirectory(this->path / name);
}

AutoPointer<const Directory> _stdxx_::WindowsDirectory::GetSubDirectory(const String & name) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return AutoPointer<const Directory>();
}

uint64 _stdxx_::WindowsDirectory::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint64();
}

uint64 _stdxx_::WindowsDirectory::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint64();
}