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
//Namespaces
using namespace StdXX;

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
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return false;
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

		void Move(const Path &from, const Path &to)
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	} windowsFileSystem;

	return windowsFileSystem;
}