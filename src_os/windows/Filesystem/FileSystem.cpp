/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/FileSystem.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace StdXX;
//Definitions
#undef CreateFile

//Class functions
FileSystem &FileSystem::GetOSFileSystem()
{
	static class WindowsFileSystem : public FileSystem
	{
	public:
		//Constructor
		WindowsFileSystem() : FileSystem(nullptr)
		{}

		//Methods
		UniquePointer<OutputStream> CreateFile(const Path &filePath)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		bool Exists(const Path &path) const
		{
			return GetFileAttributesW((LPCWSTR)path.GetString().ToUTF16().GetRawZeroTerminatedData()) != INVALID_FILE_ATTRIBUTES;
		}

		void Flush()
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		AutoPointer<Directory> GetDirectory(const Path &directoryPath)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
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
			DWORD result = GetFileAttributesW((LPCWSTR)path.GetString().ToUTF16().GetRawZeroTerminatedData());
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