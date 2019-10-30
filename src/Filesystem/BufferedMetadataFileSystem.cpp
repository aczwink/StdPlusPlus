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
//Class header
#include <Std++/Filesystem/BufferedMetadataFileSystem.hpp>
//Namespaces
using namespace StdXX;

//Local functions
template<typename T>
static AutoPointer<T> GetNodeImpl(const Path& path, AutoPointer<T> root)
{
	if(path.IsRoot())
		return root;

	Path leftPath = path;
	if(leftPath.IsAbsolute()) //skip root slash
		leftPath = leftPath.GetString().SubString(1);
	AutoPointer<T> node = root;
	while(true)
	{
		Path remaining;
		String name = leftPath.SplitOutmostPathPart(remaining);
		leftPath = remaining;

		ASSERT(node->GetType() == FileSystemNodeType::Directory, u8"Only directories can have children");
		using dirType = typename Conditional<IsConst<T>::value, const Directory, Directory>::type;
		node = node.template Cast<dirType>()->GetChild(name);

		if(leftPath.GetString().IsEmpty())
			break;
	}

	return node;
}

//Public methods
AutoPointer<FileSystemNode> BufferedMetadataFileSystem::GetNode(const Path &path)
{
	return GetNodeImpl<FileSystemNode>(path, this->GetRoot());
}

AutoPointer<const FileSystemNode> BufferedMetadataFileSystem::GetNode(const Path &path) const
{
	return GetNodeImpl<const FileSystemNode>(path, this->GetRoot());
}