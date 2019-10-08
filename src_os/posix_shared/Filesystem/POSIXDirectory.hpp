/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/Directory.hpp>

namespace _stdxx_
{
	class POSIXDirectory : public StdXX::Directory
	{
	public:
		//Constructor
		inline POSIXDirectory(const StdXX::Path &path) : path(path)
		{
		}

		//Methods
		StdXX::UniquePointer<StdXX::OutputStream> CreateFile(const StdXX::String &name) override;
		void CreateSubDirectory(const StdXX::String &name) override;
		bool Exists(const StdXX::Path &path) const override;
		StdXX::AutoPointer<FileSystemNode> GetChild(const StdXX::String &name) override;
		StdXX::AutoPointer<const FileSystemNode> GetChild(const StdXX::String &name) const override;
		StdXX::FileSystem *GetFileSystem() override;
		const StdXX::FileSystem *GetFileSystem() const override;
		StdXX::AutoPointer<const Directory> GetParent() const override;
		StdXX::Path GetPath() const override;
		bool IsEmpty() const override;
		StdXX::FileSystemNodeInfo QueryInfo() const override;

		//For range-based loop
		StdXX::DirectoryIterator begin() const override;
		StdXX::DirectoryIterator end() const override;

	private:
		//Members
		StdXX::Path path;
	};
}