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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Path.hpp"
#include "Node.hpp"
#include "File.hpp"

namespace StdXX::FileSystem
{
	struct SpaceInfo
	{
		uint64 freeSize;
		uint64 totalSize;
	};

	class ReadableFileSystem
	{
	public:
		//Destructor
		virtual ~ReadableFileSystem() = default;

		//Abstract
		virtual bool Exists(const Path &path) const = 0;
		/**
		 * Get the node identified by path or nullptr if not existent.
		 *
		 * @param path - If relative, it should be considered relative to the root.
		 * @return
		 */
		virtual AutoPointer<const Node> GetNode(const Path& path) const = 0;
		virtual SpaceInfo QuerySpace() const = 0;

		//Inline
		inline AutoPointer<const Directory> GetDirectory(const Path& path) const
		{
			AutoPointer<const Node> node = this->GetNode(path);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == NodeType::Directory, u8"Node is not a directory.");

			return node.MoveCast<const Directory>();
		}

		inline AutoPointer<const File> GetFile(const Path& path) const
		{
			AutoPointer<const Node> node = this->GetNode(path);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == NodeType::File, u8"Node is not a file.");

			return node.MoveCast<const File>();
		}

		inline bool IsDirectory(const Path& path) const
		{
			return this->GetNode(path)->GetType() == NodeType::Directory;
		}
	};
}