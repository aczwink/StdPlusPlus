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
//Class header
#include <Std++/FileSystem/Buffered/BufferedMetadataFileSystem.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

UniquePointer<DirectoryEnumerator> BufferedMetadataFileSystem::EnumerateChildren(const Path &path) const
{
	class ContainerDirectoryIteratorState : public DirectoryEnumerator
	{
	public:
		//Constructor
		ContainerDirectoryIteratorState(const MemoryDirectory &dir) : dir(dir), childrenIterator(this->dir.Children().begin())
		{
			this->first = true;
		}

		//Public methods
		const DirectoryEntry &GetCurrent() const override
		{
			return this->directoryEntry;
		}

		bool MoveForward() override
		{
			if(this->first)
				this->first = false;
			else
				++this->childrenIterator;

			if(this->IsAtEnd())
				return false;

			this->directoryEntry.name = (*this->childrenIterator).key;
			this->directoryEntry.type = this->DeriveType((*this->childrenIterator).value);

			return true;
		}

	private:
		//Members
		const MemoryDirectory &dir;
		bool first;
		ConstMapIterator<String, UniquePointer<FileMetadataNode>> childrenIterator;
		DirectoryEntry directoryEntry;

		//Inline
		inline bool IsAtEnd() const
		{
			return (this->childrenIterator == this->dir.Children().end());
		}

		inline FileType DeriveType(const UniquePointer<FileMetadataNode>& node)
		{
			if(node.IsInstanceOf<const MemoryDirectory>())
				return FileType::Directory;
			return FileType::File;
		}
	};

	const FileMetadataNode* node = this->FindNode(path);
	if(node == nullptr)
		return nullptr;
	if(IS_INSTANCE_OF(node, MemoryDirectory))
		return new ContainerDirectoryIteratorState((const MemoryDirectory &) *node);
	return nullptr;
}

Optional <FileInfo> BufferedMetadataFileSystem::QueryFileInfo(const Path &path) const
{
	const FileMetadataNode* node = this->FindNode(path);
	if(node == nullptr)
		return {};

	return node->Info();
}

//Protected methods
const FileMetadataNode* BufferedMetadataFileSystem::FindNode(const Path &path) const
{
	if (path.IsRoot())
		return this->GetRoot();

	Path leftPath = path;
	if (leftPath.IsAbsolute()) //skip root slash
		leftPath = leftPath.String().SubString(1);
	const FileMetadataNode* node = this->GetRoot();
	do
	{
		Path remaining;
		String name = leftPath.SplitOutmostPathPart(remaining);
		leftPath = remaining;

		ASSERT(IS_INSTANCE_OF(node, MemoryDirectory), u8"Only directories can have children");
		node = ((MemoryDirectory*)node)->GetChild(name);
	}
	while ( !((node == nullptr) || leftPath.String().IsEmpty()) );

	return node;
}
