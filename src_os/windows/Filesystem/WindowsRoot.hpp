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
//Local
#include <Std++/Filesystem/Directory.hpp>
namespace _stdxx_
{
	class WindowsRoot : public StdXX::Directory
	{
	public:
		//Methods
		virtual bool ContainsFile(const StdXX::String & name) const override;
		virtual bool ContainsSubDirectory(const StdXX::String & name) const override;
		virtual StdXX::UniquePointer<StdXX::OutputStream> CreateFile(const StdXX::String & name) override;
		virtual void CreateSubDirectory(const StdXX::String & name) override;
		virtual bool Exists(const StdXX::Path & path) const override;
		virtual StdXX::AutoPointer<const StdXX::File> GetFile(const StdXX::String & name) const override;
		virtual StdXX::FileSystem * GetFileSystem() override;
		virtual const StdXX::FileSystem * GetFileSystem() const override;
		virtual StdXX::AutoPointer<const Directory> GetParent() const override;
		virtual StdXX::Path GetPath() const override;
		virtual StdXX::AutoPointer<Directory> GetSubDirectory(const StdXX::String & name) override;
		virtual StdXX::AutoPointer<const Directory> GetSubDirectory(const StdXX::String & name) const override;
		uint64 GetStoredSize() const override;
		virtual uint64 GetSize() const override;
		virtual StdXX::DirectoryIterator begin() const override;
		virtual StdXX::DirectoryIterator end() const override;
	};
}