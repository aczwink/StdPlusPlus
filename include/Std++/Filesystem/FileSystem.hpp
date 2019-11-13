/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "../SmartPointers/AutoPointer.hpp"
#include "../SmartPointers/UniquePointer.hpp"
#include "Directory.hpp"
#include "File.hpp"

namespace StdXX
{
	//Move declarations
	class FileSystemFormat;

	class STDPLUSPLUS_API FileSystem
	{
	public:
		//Constructor
		inline FileSystem(const FileSystemFormat *format) : fileSystemFormat(format)
		{
		}

		//Destructor
		virtual ~FileSystem()
		{
		}

		//Abstract
		virtual UniquePointer<OutputStream> CreateFile(const Path &filePath) = 0;
		virtual bool Exists(const Path &path) const = 0;
		virtual void Flush() = 0;
		/**
		 * Get the node identified by path or nullptr if not existent.
		 *
		 * @param path - If relative, it should be considered relative to the root.
		 * @return
		 */
		virtual AutoPointer<FileSystemNode> GetNode(const Path& path) = 0;
		virtual AutoPointer<const FileSystemNode> GetNode(const Path& path) const = 0;
		virtual AutoPointer<Directory> GetRoot() = 0;
		virtual AutoPointer<const Directory> GetRoot() const = 0;
		virtual uint64 GetSize() const = 0;
		virtual void Move(const Path &from, const Path &to) = 0;

		//Methods
		void CreateDirectoryTree(const Path &directoryPath);

		//Functions
		/**
		 * Create the file system at the given path.
		 * The path must not refer to an existing node on the filesystem.
		 * The filesystem is opened in writable mode.
		 * @param id
		 * @param path
		 */
		static UniquePointer<FileSystem> Create(const String &id, const Path &path);
		/**
		 * Loads an existing file system in writable mode.
		 * The file system must exist.
		 * @param p
		 * @return
		 */
		static UniquePointer<FileSystem> LoadFromFile(const Path &p);
		/**
		 * Loads an existing file system in read-only mode.
		 * The file system must exist.
		 * @param path
		 * @return
		 */
		static UniquePointer<const FileSystem> LoadFromFileReadOnly(const Path& path);

		//Inline
		inline AutoPointer<Directory> GetDirectory(const Path& path)
		{
			AutoPointer<FileSystemNode> node = this->GetNode(path);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == FileSystemNodeType::Directory, u8"Node is not a directory.");

			return node.MoveCast<Directory>();
		}

		inline AutoPointer<const File> GetFile(const Path& path) const
		{
			AutoPointer<const FileSystemNode> node = this->GetNode(path);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == FileSystemNodeType::File, u8"Node is not a file.");

			return node.MoveCast<const File>();
		}

		inline const FileSystemFormat *GetFormat() const
		{
			return this->fileSystemFormat;
		}

		inline bool IsDirectory(const Path& path) const
		{
			return this->GetNode(path)->GetType() == FileSystemNodeType::Directory;
		}

	private:
		//Members
		const FileSystemFormat *fileSystemFormat;
	};
}