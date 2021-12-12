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
#include <Std++/FileSystem/FileSystemsManager.hpp>
//Local
#include <Std++/FileSystem/OSFileSystem.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

class WindowsFileSystem : public OSFileSystem
{
	//TODO: NOT IMPLEMENTED
	virtual UniquePointer<DirectoryEnumerator> EnumerateChildren(const Path& path) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return UniquePointer<DirectoryEnumerator>();
	}
	virtual UniquePointer<InputStream> OpenFileForReading(const Path& path, bool verify) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return UniquePointer<InputStream>();
	}
	virtual Optional<FileInfo> QueryFileInfo(const Path& path) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return Optional<FileInfo>();
	}
	virtual SpaceInfo QuerySpace() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return SpaceInfo();
	}
	virtual Optional<Path> ReadLinkTarget(const Path& path) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return Optional<Path>();
	}
	virtual void ChangePermissions(const Path& path, const FileSystem::Permissions& newPermissions) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	virtual Optional<Errors::CreateDirectoryError> CreateDirectory(const Path& path, const Permissions* permissions = nullptr) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return Optional<Errors::CreateDirectoryError>();
	}
	virtual UniquePointer<OutputStream> CreateFile(const Path& filePath) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return UniquePointer<OutputStream>();
	}
	virtual void CreateLink(const Path& linkPath, const Path& linkTargetPath) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	virtual void DeleteFile(const Path& path) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	virtual void Flush() override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	virtual void Move(const Path& from, const Path& to) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
	virtual UniquePointer<OutputStream> OpenFileForWriting(const Path& path) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return UniquePointer<OutputStream>();
	}
	virtual void RemoveDirectory(const Path& path) override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
};

class OSFileSystem& FileSystemsManager::OSFileSystem()
{
	static WindowsFileSystem windows_fs;
	return windows_fs;
}