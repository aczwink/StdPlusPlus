/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/WritableFileSystem.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include "MemoryDirectory.hpp"
#include "MemoryFile.hpp"

namespace StdXX::FileSystem
{
	/**
	 * This class is for writing one-time written archives.
	 * It will store all content of the filesystem until it is flushed. At that time the archive is written.
	 * If the filesystem is flushed multiple times, it is rewritten each time.
	 */
	class ArchiveCreationFileSystem : public WritableFileSystem
	{
	public:
		//Constructor
		inline ArchiveCreationFileSystem(UniquePointer<FileOutputStream>&& outputStream) : root(new MemoryDirectory), outputStream(StdXX::Move(outputStream))
		{
			this->isFlushed = false;
		}

		//Methods
		void ChangePermissions(const Path &path, const FileSystem::Permissions &newPermissions) override;
		Optional<Errors::CreateDirectoryError> CreateDirectory(const Path &path, const Permissions *permissions) override;
		UniquePointer <OutputStream> CreateFile(const Path &filePath) override;
		void CreateLink(const Path &linkPath, const Path &linkTargetPath) override;
		void DeleteFile(const Path &path) override;
		void Flush() override;
		void Move(const Path &from, const Path &to) override;
		UniquePointer<OutputStream> OpenFileForWriting(const Path &path) override;
		void RemoveDirectory(const Path &path) override;

	protected:
		//Methods
		BinaryTreeMap<Path, const MemoryFile*> TraverseFiles() const;

	private:
		//State
		bool isFlushed;
		UniquePointer<MemoryDirectory> root;
		UniquePointer<FileOutputStream> outputStream;

		//Abstract
		virtual void WriteContainer(SeekableOutputStream& outputStream) = 0;

		//Methods
		MemoryDirectory* GetDirectory(const Path& dirPath);
	};
}