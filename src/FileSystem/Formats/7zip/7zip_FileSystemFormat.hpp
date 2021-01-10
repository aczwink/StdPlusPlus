/*
* Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/FileSystem/Format.hpp>

namespace _stdxx_
{
	class SevenZip_FileSystemFormat : public StdXX::FileSystem::Format
	{
	public:
		//Methods
		StdXX::FileSystem::RWFileSystem* CreateFileSystem(const StdXX::FileSystem::Path & fileSystemPath) const override;
		StdXX::String GetId() const override;
		StdXX::String GetName() const override;
		float32 Matches(StdXX::SeekableInputStream & inputStream) const override;

		StdXX::FileSystem::RWFileSystem *OpenFileSystem(const StdXX::FileSystem::Path &fileSystemPath,
														const StdXX::FileSystem::OpenOptions& options) const override;
		StdXX::FileSystem::ReadableFileSystem *OpenFileSystemReadOnly(const StdXX::FileSystem::Path &fileSystemPath,
																const StdXX::FileSystem::OpenOptions& options) const override;
	};
}