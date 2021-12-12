/*
* Copyright (c) 2017-2019,2021 Amir Czwink (amir130@hotmail.de)
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
//Definitions
#undef CreateFile
//Local
#include "WindowsDirectory.hpp"
#include "WindowsRoot.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Local functions
static String ToAbsolutePath(const String &path)
{
	const uint32 nCodeUnits = 4096;
	WCHAR buffer[nCodeUnits];

	GetFullPathNameW((LPCWSTR)path.ToUTF16().GetRawZeroTerminatedData(), nCodeUnits, buffer, nullptr);

	return u8"/" + String::CopyRawString((uint16 *)buffer);
}

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

	return ::ToAbsolutePath(path.String());
}

String OSFileSystem::ToNativePath(const Path &path) const
{
	String winPath = path.String();

	//remove the root "/"
	if (path.IsAbsolute())
		winPath = winPath.SubString(1);

	//on windows we are not forced to use the backslash as seperator. Forward slash is also fine
	//tmp = tmp.Replace(u8"/", u8"\\");

	return winPath.ToUTF16();
}

//Class functions
/*OSFileSystem& OSFileSystem::Instance()
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
			if(directoryPath.IsRoot())
				return new WindowsRoot;
			return new WindowsDirectory(directoryPath);
		}

		AutoPointer<const File> GetFile(const Path& filePath) const
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		AutoPointer<Directory> GetRoot()
		{
			return new WindowsRoot;
		}

		uint64 GetSize() const
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return 0;
		}

		bool IsDirectory(const Path &path) const override
		{
			//the root (i.e. disk names list) is a directory
			if (path.IsRoot())
				return true;

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
}*/