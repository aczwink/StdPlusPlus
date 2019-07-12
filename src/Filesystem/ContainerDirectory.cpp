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
//Class header
#include <Std++/Filesystem/ContainerDirectory.hpp>
//Local
#include <Std++/Filesystem/DirectoryIterator.hpp>
#include <Std++/Filesystem/ContainerFileSystem.hpp>
#include <Std++/Filesystem/MemoryFile.hpp>
//Namespaces
using namespace StdXX;

//Public methods
UniquePointer<OutputStream> ContainerDirectory::CreateFile(const String &name)
{
	MemoryFile* file = new MemoryFile;
	this->children[name] = file;

	this->fileSystem->isFlushed = false;

	return file->OpenForWriting();
}

void ContainerDirectory::CreateSubDirectory(const String &name)
{
	this->children[name] = new ContainerDirectory(name, this);

	this->fileSystem->isFlushed = false;
}

bool ContainerDirectory::Exists(const Path &path) const
{
	Path leftPart;
	Path child = path.SplitOutmostPathPart(leftPart);
	const String &childString = child.GetString();
	if(leftPart.GetString().IsEmpty())
		return this->children.Contains(childString);

	if(this->HasSubDirectory(childString))
		return this->children[childString].Cast<ContainerDirectory>()->Exists(leftPart);
	return false;
}

AutoPointer<FileSystemNode> ContainerDirectory::GetChild(const String &name)
{
	if(!this->children.Contains(name))
		return nullptr;
	return this->children[name];
}

AutoPointer<const FileSystemNode> ContainerDirectory::GetChild(const String &name) const
{
	if(!this->children.Contains(name))
		return nullptr;
	return this->children[name];
}

FileSystem *ContainerDirectory::GetFileSystem()
{
	return this->fileSystem;
}

const FileSystem *ContainerDirectory::GetFileSystem() const
{
	return this->fileSystem;
}

AutoPointer<const Directory> ContainerDirectory::GetParent() const
{
	return this->parent;
}

Path ContainerDirectory::GetPath() const
{
	if(this->parent)
		return this->parent->GetPath() / this->name;

	return Path(u8"/");
}

uint64 ContainerDirectory::GetSize() const
{
	uint64 sum = 0;
	for(const auto &kv : this->children)
		sum += kv.value->GetSize();

	return sum;
}

uint64 ContainerDirectory::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

//For range-based loop
DirectoryIterator ContainerDirectory::begin() const
{
	class ContainerDirectoryIteratorState : public _stdxx_::DirectoryIteratorState
	{
	public:
		//Constructor
		ContainerDirectoryIteratorState(const ContainerDirectory &dir)
			: dir(dir), childrenIterator(dir.children.begin())
		{
		}

		//Public methods
		bool Equals(DirectoryIteratorState *other) const override
		{
			if(other == nullptr)
				return this->IsAtEnd();

			ContainerDirectoryIteratorState *otherTyped = dynamic_cast<ContainerDirectoryIteratorState *>(other);
			if(otherTyped)
			{
				return (&this->dir == &otherTyped->dir) &&
					(this->childrenIterator == otherTyped->childrenIterator);
			}

			return false;
		}

		String GetCurrent() override
		{
			const auto& kv = (*this->childrenIterator);
			return kv.key;
		}

		void Next() override
		{
			++this->childrenIterator;
		}

	private:
		//Members
		const ContainerDirectory &dir;
		ConstMapIterator<String, AutoPointer<FileSystemNode>> childrenIterator;

		//Inline
		inline bool IsAtEnd() const
		{
			return (this->childrenIterator == this->dir.children.end());
		}
	};

	return DirectoryIterator(new ContainerDirectoryIteratorState(*this));
}

DirectoryIterator ContainerDirectory::end() const
{
	return DirectoryIterator(nullptr);
}

//Private methods
void ContainerDirectory::AddSourceFile(String fileName, const ContainerFileHeader& header)
{
	ContainerFile *file = new ContainerFile(header, this->fileSystem);
	this->children[fileName] = file;
}