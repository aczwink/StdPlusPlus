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
#pragma once
//Local
#include <Std++/Errorhandling/Exceptions/FileAlreadyExistsException.hpp>
#include <Std++/Errorhandling/Exceptions/PermissionDeniedException.hpp>
#include <Std++/Errorhandling/Errors/IllegalCodePathError.hpp>
#include "OSFileSystem.hpp"
#include "FileSystemsManager.hpp"
#include "ReadOnlyFile.hpp"

namespace StdXX::FileSystem
{
	class File : public ReadOnlyFile
	{
	public:
		//Constructors
		inline File(const class Path& path) : File(FileSystemsManager::Instance().OSFileSystem(), path)
		{
		}

		inline File(RWFileSystem& fileSystem, const class Path& path) : ReadOnlyFile(fileSystem, path), fileSystem(fileSystem)
		{
		}

		//Inline
		inline void ChangePermissions(const FileSystem::Permissions& newPermissions)
		{
			this->fileSystem.ChangePermissions(this->path, newPermissions);
		}

		inline void CreateDirectory()
		{
			auto result = this->fileSystem.CreateDirectory(this->path);
			if(result.HasValue())
			{
				switch(*result)
				{
					case Errors::CreateDirectoryError::FileExists:
						throw ErrorHandling::FileAlreadyExistsException(this->path);
					case Errors::CreateDirectoryError::PermissionsDenied:
						throw ErrorHandling::PermissionDeniedException(this->path);
					default:
						RAISE(ErrorHandling::IllegalCodePathError);
				}
			}
			this->InvalidateCache();
		}

		inline void CreateDirectories()
		{
			if(this->path.IsRoot())
				return;
			if(!this->Exists())
			{
				File parent(this->fileSystem, this->path.GetParent());
				parent.CreateDirectories();
				this->CreateDirectory();
			}
			ASSERT_EQUALS(FileType::Directory, this->Type());
		}

		inline void CreateLink(const class Path& linkTargetPath)
		{
			this->fileSystem.CreateLink(this->path, linkTargetPath);
		}

		inline void DeleteFile()
		{
			this->fileSystem.DeleteFile(this->path);
		}

		inline UniquePointer<OutputStream> OpenForWriting()
		{
			return this->fileSystem.OpenFileForWriting(this->path);
		}

		inline void RemoveDirectory()
		{
			this->fileSystem.RemoveDirectory(this->path);
		}

		inline void RemoveChildrenRecursively()
		{
			for(const DirectoryEntry& directoryEntry : *this)
			{
				File child(this->fileSystem, this->path / directoryEntry.name);
				if(directoryEntry.type == FileType::Directory)
					child.RemoveDirectoryRecursively();
				else
					child.DeleteFile();
			}
		}

		inline void RemoveDirectoryRecursively()
		{
			this->RemoveChildrenRecursively();
			this->RemoveDirectory();
		}

	private:
		//Members
		RWFileSystem& fileSystem;
	};
}