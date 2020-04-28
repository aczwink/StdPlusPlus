/*
* Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/FileSystem/MemoryDirectory.hpp>

namespace _stdxx_
{
	//Forward declarations
	class ZipFileSystem;

	class ZipDirectory : public StdXX::FileSystem::MemoryDirectory
	{
	public:
		//Constructor
		inline ZipDirectory(ZipFileSystem& fileSystem) : fileSystem(fileSystem)
		{
		}

		//Methods
		void ChangePermissions(const StdXX::FileSystem::NodePermissions &newPermissions) override;
		StdXX::UniquePointer<StdXX::OutputStream> CreateFile(const StdXX::String &name) override;
		void CreateSubDirectory(const StdXX::String &name, const StdXX::FileSystem::NodePermissions* permissions) override;
		bool Exists(const StdXX::FileSystem::Path &path) const override;
		bool IsEmpty() const override;
		StdXX::NodeInfo QueryInfo() const override;

	private:
		//Members
		ZipFileSystem& fileSystem;
	};
}