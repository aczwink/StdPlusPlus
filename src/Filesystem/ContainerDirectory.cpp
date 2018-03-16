/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace StdPlusPlus;

//Public methods
bool ContainerDirectory::ContainsFile(const String &name) const
{
	return this->files.Contains(name);
}

bool ContainerDirectory::ContainsSubDirectory(const String &name) const
{
	return this->subDirectories.Contains(name);
}

UniquePointer<OutputStream> ContainerDirectory::CreateFile(const String &name)
{
	ContainerFile *file = new ContainerFile(name, this);
	this->files[name] = file;

	this->fileSystem->isFlushed = false;

	return file->OpenForWriting();
}

void ContainerDirectory::CreateSubDirectory(const String &name)
{
	this->subDirectories[name] = new ContainerDirectory(name, this);

	this->fileSystem->isFlushed = false;
}

bool ContainerDirectory::Exists(const Path &path) const
{
	Path leftPart;
	Path child = path.SplitOutmostPathPart(leftPart);
	const String &childString = child.GetString();
	if(leftPart.GetString().IsEmpty())
		return this->subDirectories.Contains(childString) || this->files.Contains(childString);

	if(this->subDirectories.Contains(childString))
		return this->subDirectories[childString]->Exists(leftPart);
	return false;
}

FileSystem *ContainerDirectory::GetFileSystem()
{
	return this->fileSystem;
}

const FileSystem *ContainerDirectory::GetFileSystem() const
{
	return this->fileSystem;
}

String ContainerDirectory::GetName() const
{
	return this->name;
}

AutoPointer<Directory> ContainerDirectory::GetParent() const
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
	for(const auto &kv : this->files)
		sum += kv.value->GetSize();
	for(const auto &kv : this->subDirectories)
		sum += kv.value->GetSize();

	return sum;
}

AutoPointer<Directory> ContainerDirectory::GetSubDirectory(const String &name)
{
	return this->subDirectories[name].Cast<Directory>();
}

AutoPointer<const Directory> ContainerDirectory::GetSubDirectory(const String &name) const
{
	return this->subDirectories[name].Cast<const Directory>();
}

//For range-based loop
DirectoryIterator ContainerDirectory::begin() const
{
	class ContainerDirectoryIteratorState : public _stdpp::DirectoryIteratorState
	{
	public:
		//Constructor
		ContainerDirectoryIteratorState(const ContainerDirectory &dir)
			: dir(dir), dirsIterator(dir.subDirectories.begin()), filesIterator(dir.files.begin())
		{
			this->atDirs = this->dirsIterator != dir.subDirectories.end();
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
					(this->atDirs == otherTyped->atDirs) &&
					(this->dirsIterator == otherTyped->dirsIterator) &&
					(this->filesIterator == otherTyped->filesIterator);
			}

			return false;
		}

		AutoPointer<FileSystemNode> GetCurrent() const override
		{
			if(this->atDirs)
				return (*this->dirsIterator).value.Cast<FileSystemNode>();
			return (*this->filesIterator).value.Cast<FileSystemNode>();
		}

		void Next() override
		{
			if(this->atDirs)
			{
				++this->dirsIterator;
				if(this->dirsIterator == this->dir.subDirectories.end())
					this->atDirs = false;
			}
			else
			{
				++this->filesIterator;
			}
		}

	private:
		//Members
		bool atDirs;
		const ContainerDirectory &dir;
		ConstMapIterator<String, AutoPointer<ContainerDirectory>> dirsIterator;
		ConstMapIterator<String, AutoPointer<ContainerFile>> filesIterator;

		//Inline
		inline bool IsAtEnd() const
		{
			return (this->dirsIterator == this->dir.subDirectories.end()) && (this->filesIterator == this->dir.files.end());
		}
	};

	return DirectoryIterator(new ContainerDirectoryIteratorState(*this));
}

DirectoryIterator ContainerDirectory::end() const
{
	return DirectoryIterator(nullptr);
}

//Private methods
void ContainerDirectory::AddSourceFile(String fileName, uint64 offset, uint64 size)
{
	ContainerFile *file = new ContainerFile(fileName, this);
	file->offset = offset;
	file->size = size;
	this->files[fileName] = file;
}