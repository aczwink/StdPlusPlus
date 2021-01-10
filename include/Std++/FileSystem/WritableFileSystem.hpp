/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Path.hpp"
#include "Node.hpp"

namespace StdXX::FileSystem
{
	class WritableFileSystem
	{
	public:
		//Destructor
		virtual ~WritableFileSystem() = default;

		//Abstract
		virtual UniquePointer<OutputStream> CreateFile(const Path &filePath) = 0;
		virtual void CreateLink(const Path& linkPath, const Path& linkTargetPath) = 0;
		virtual void Flush() = 0;
		/**
		 * Get the node identified by path or nullptr if not existent.
		 *
		 * @param path - If relative, it should be considered relative to the root.
		 * @return
		 */
		virtual AutoPointer<Node> GetNode(const Path& path) = 0;
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
		static UniquePointer<WritableFileSystem> Create(const String &id, const Path &path);

		//Inline
		inline AutoPointer<Directory> GetDirectory(const Path& path)
		{
			AutoPointer<Node> node = this->GetNode(path);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == NodeType::Directory, u8"Node is not a directory.");

			return node.MoveCast<Directory>();
		}
	};
}