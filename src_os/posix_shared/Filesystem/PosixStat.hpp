/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/FileSystemNodeInfo.hpp>
#include <Std++/Filesystem/Path.hpp>
#include <Std++/SmartPointers/AutoPointer.hpp>
#include <Std++/Filesystem/FileSystemNode.hpp>

namespace _stdxx_
{
	//Functions
	template<typename ConstOrNotFileSystemNode = const StdXX::FileSystemNode>
	StdXX::AutoPointer<ConstOrNotFileSystemNode> StatFindNode(const StdXX::Path& path);
	StdXX::FileSystemNodeInfo StatQueryFileInfo(const StdXX::Path &path, uint64 &fileSize);

	//Inline
	inline StdXX::FileSystemNodeInfo StatQueryFileInfo(const StdXX::Path &path)
	{
		uint64 fileSize;
		return StatQueryFileInfo(path, fileSize);
	}
}