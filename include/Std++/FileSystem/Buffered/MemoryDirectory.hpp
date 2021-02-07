/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Map/Map.hpp>

namespace StdXX::FileSystem
{
	/**
	 * A directory that is completely stored in memory.
	 */
	class MemoryDirectory : public MemoryFileNode
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
		inline const auto& Children() const
		{
			return this->children;
		}

		//Inline
		inline void AddChild(const String& nodeName, MemoryFileNode* node)
		{
			ASSERT(!this->children.Contains(nodeName), u8"Can't add child that already exists.");
			this->children[nodeName] = node;
		}

		const FileInfo &Info() const override
		{
			return this->fileInfo;
		}

		AutoPointer<MemoryFileNode> GetChild(const String &name)
		{
			if(!this->children.Contains(name))
				return nullptr;
			return this->children[name];
		}

		inline AutoPointer<const MemoryFileNode> GetChild(const String &name) const
		{
			if(!this->children.Contains(name))
				return nullptr;
			return this->children[name];
		}

	private:
		//Members
		Map<String, AutoPointer<MemoryFileNode>> children;
		FileInfo fileInfo;
	};
}