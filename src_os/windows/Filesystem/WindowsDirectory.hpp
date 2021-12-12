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
/*
//Local
#include <Std++/Filesystem/Directory.hpp>

namespace _stdxx_
{
	class WindowsDirectory : public StdXX::Directory
	{
	public:
		//Constructor
		inline WindowsDirectory(const StdXX::Path &path) : path(path)
		{
		}

		//Methods
		bool ContainsFile(const StdXX::String & name) const override;
		bool ContainsSubDirectory(const StdXX::String & name) const override;
		StdXX::UniquePointer<StdXX::OutputStream> CreateFile(const StdXX::String & name) override;
		void CreateSubDirectory(const StdXX::String & name) override;
		bool Exists(const StdXX::Path & path) const override;
		StdXX::AutoPointer<const StdXX::File> GetFile(const StdXX::String & name) const override;
		StdXX::FileSystem * GetFileSystem() override;
		const StdXX::FileSystem * GetFileSystem() const override;
		StdXX::AutoPointer<const Directory> GetParent() const override;
		StdXX::Path GetPath() const override;
		StdXX::AutoPointer<Directory> GetSubDirectory(const StdXX::String & name) override;
		StdXX::AutoPointer<const Directory> GetSubDirectory(const StdXX::String & name) const override;
		uint64 GetStoredSize() const override;
		uint64 GetSize() const override;
		StdXX::DirectoryIterator begin() const override;
		StdXX::DirectoryIterator end() const override;

	private:
		//Members
		StdXX::Path path;
	};
}*/