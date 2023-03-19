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
#include <Std++/FileSystem/ReadableFileSystem.hpp>
#include "FileMetadataNode.hpp"
#include "MemoryDirectory.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A filesystem that stores all nodes (but not their content) in memory.
	 */
	class BufferedMetadataFileSystem : public ReadableFileSystem
	{
	public:
		//Methods
		UniquePointer <DirectoryEnumerator> EnumerateChildren(const Path &path) const override;
		Optional <FileInfo> QueryFileInfo(const Path &path) const override;

	protected:
		/**
		 * Get the node identified by path or nullptr if not existent.
		 *
		 * @param path - If relative, it should be considered relative to the root.
		 * @return
		 */
		const FileMetadataNode* FindNode(const Path& path) const;

		//Abstract
		virtual const MemoryDirectory* GetRoot() const = 0;

		//Inline
		/*inline AutoPointer <FileMetadataNode> FindNodeNonConst(const Path& path)
		{
			return this->FindNode(path).MoveConstCast();
		}*/
	};
}