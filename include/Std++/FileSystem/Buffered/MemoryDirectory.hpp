/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "FileMetadataNode.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A directory that is completely stored in memory.
	 */
	class MemoryDirectory : public FileMetadataNode
	{
	public:
		//Constructors
		inline MemoryDirectory()
		{
			this->fileInfo.type = FileType::Directory;
		}

		inline MemoryDirectory(FileInfo&& fileInfo) : fileInfo(Move(fileInfo))
		{
		}

		//Properties
		inline const BinaryTreeMap<String, UniquePointer<FileMetadataNode>>& Children() const
		{
			return this->children;
		}

		//Inline
		const FileInfo &Info() const override
		{
			return this->fileInfo;
		}

		FileMetadataNode* GetChild(const String &name)
		{
			if(!this->children.Contains(name))
				return nullptr;
			return this->children[name].operator->();
		}

		inline const FileMetadataNode* GetChild(const String &name) const
		{
			if(!this->children.Contains(name))
				return nullptr;
			return this->children[name].operator->();
		}

		inline void SetChild(const String &nodeName, UniquePointer<FileMetadataNode> &&node)
		{
			this->children[nodeName] = Move(node);
		}

	private:
		//Members
		BinaryTreeMap<String, UniquePointer<FileMetadataNode>> children;
		FileInfo fileInfo;
	};
}