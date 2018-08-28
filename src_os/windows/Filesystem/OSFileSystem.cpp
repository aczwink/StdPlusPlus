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
//Class header
#include <Std++/Filesystem/OSFileSystem.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
//Namespaces
using namespace StdXX;
//Definitions
#undef CreateFile

//Local functions
static String ToAbsolutePath(const String &path)
{
	const uint32 nCodeUnits = 4096;
	WCHAR buffer[nCodeUnits];

	GetFullPathNameW((LPCWSTR)path.ToUTF16().GetRawZeroTerminatedData(), nCodeUnits, buffer, nullptr);

	return u8"/" + String::CopyRawString((uint16 *)buffer);
}

//Directory class
static class WindowsDirectory : public Directory
{
public:
	//Constructor
	inline WindowsDirectory(const Path &path) : path(path)
	{
	}

	//Methods
	bool ContainsFile(const String & name) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return false;
	}

	bool ContainsSubDirectory(const String & name) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return false;
	}

	UniquePointer<OutputStream> CreateFile(const String & name) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return UniquePointer<OutputStream>();
	}

	void CreateSubDirectory(const String & name) override
	{
		String nativePath = OSFileSystem::GetInstance().ToNativePath((this->path / name).GetString());
		CreateDirectoryW((LPCWSTR)nativePath.GetRawZeroTerminatedData(), NULL);
	}

	bool Exists(const Path & path) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return false;
	}

	FileSystem * GetFileSystem() override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	const FileSystem * GetFileSystem() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	String GetName() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return String();
	}

	AutoPointer<Directory> GetParent() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return AutoPointer<Directory>();
	}

	Path GetPath() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return Path();
	}

	uint64 GetSize() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return uint64();
	}
	
	AutoPointer<Directory> GetSubDirectory(const String & name) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return AutoPointer<Directory>();
	}
	
	AutoPointer<const Directory> GetSubDirectory(const String & name) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return AutoPointer<const Directory>();
	}

	//For range-based loop	
	DirectoryIterator begin() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return DirectoryIterator(nullptr);
	}
	
	DirectoryIterator end() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return DirectoryIterator(nullptr);
	}

private:
	//Members
	Path path;
};

//Public methods
Path OSFileSystem::FromNativePath(const String &nativePath) const
{
	return ::ToAbsolutePath(nativePath).Replace(u8"\\", u8"/");
}

Path OSFileSystem::GetWorkingDirectory() const
{
	const uint32 nCodeUnits = 4096;
	WCHAR buffer[nCodeUnits];

	GetCurrentDirectoryW(nCodeUnits, buffer);
	return this->FromNativePath(String::CopyRawString((uint16 *)buffer));
}

Path OSFileSystem::ToAbsolutePath(const Path &path) const
{
	if (path.IsAbsolute())
		return path;

	return ::ToAbsolutePath(path.GetString());
}

String OSFileSystem::ToNativePath(const Path &path) const
{
	String winPath = path.GetString();

	//remove the root "/"
	if (path.IsAbsolute())
		winPath = winPath.SubString(1);

	//on windows we are not forced to use the backslash as seperator. Forward slash is also fine
	//tmp = tmp.Replace(u8"/", u8"\\");

	return winPath.ToUTF16();
}

//Class functions
OSFileSystem &OSFileSystem::GetInstance()
{
	static class WindowsFileSystem : public OSFileSystem
	{
	public:
		//Methods
		UniquePointer<OutputStream> CreateFile(const Path &filePath)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		bool Exists(const Path &path) const
		{
			String nativePath = this->ToNativePath(path);
			return GetFileAttributesW((LPCWSTR)nativePath.GetRawZeroTerminatedData()) != INVALID_FILE_ATTRIBUTES;
		}

		void Flush()
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		AutoPointer<Directory> GetDirectory(const Path &directoryPath)
		{
			return new WindowsDirectory(directoryPath);
		}

		AutoPointer<Directory> GetRoot()
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		uint64 GetSize() const
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return 0;
		}

		bool IsDirectory(const Path &path) const override
		{
			String nativePath = this->ToNativePath(path);
			DWORD result = GetFileAttributesW((LPCWSTR)nativePath.GetRawZeroTerminatedData());
			ASSERT(result != INVALID_FILE_ATTRIBUTES, u8"TODO: throw exception that path doesn't exist");

			return (result & FILE_ATTRIBUTE_DIRECTORY) != 0;
		}

		void Move(const Path &from, const Path &to)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	} windowsFileSystem;

	return windowsFileSystem;
}