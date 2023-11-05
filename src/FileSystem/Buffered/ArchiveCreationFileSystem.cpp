/*
 * Copyright (c) 2018-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/Buffered/ArchiveCreationFileSystem.hpp>
//Local
#include <Std++/FileSystem/Buffered/MemoryFile.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Static functions
static void ProcessDir(const Path& path, const MemoryDirectory* directory, BinaryTreeMap<Path, const MemoryFile*>& result)
{
	for(const auto& child : directory->Children())
	{
		auto node = child.value.operator->();
		const auto* childDir = dynamic_cast<const MemoryDirectory *>(node);
		if(childDir)
			ProcessDir(path / child.key, childDir, result);
		else
			result.Insert(path / child.key, dynamic_cast<const MemoryFile *>(node));
	}
};

//Public methods
void ArchiveCreationFileSystem::ChangePermissions(const Path &path, const FileSystem::Permissions &newPermissions)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

Optional<Errors::CreateDirectoryError> ArchiveCreationFileSystem::CreateDirectory(const Path& path, const Permissions *permissions)
{
	MemoryDirectory* parent = this->GetDirectory(path.GetParent());

	FileInfo fileInfo;
	fileInfo.type = FileType::Directory;
	fileInfo.permissions = permissions ? permissions->Clone() : nullptr;

	parent->SetChild(path.GetName(), new MemoryDirectory(StdXX::Move(fileInfo)));
	this->isFlushed = false;

	return {};
}

UniquePointer<OutputStream> ArchiveCreationFileSystem::CreateFile(const Path& filePath)
{
	MemoryDirectory* parent = this->GetDirectory(filePath.GetParent());

	MemoryFile* file = new MemoryFile;
	parent->SetChild(filePath.GetName(), file);

	this->isFlushed = false;
	//TODO: any time data is written to the file, isFlushed should be set to false

	return file->OpenForWriting();
}

void ArchiveCreationFileSystem::CreateLink(const Path &linkPath, const Path &linkTargetPath)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void ArchiveCreationFileSystem::DeleteFile(const Path &path)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void ArchiveCreationFileSystem::Flush()
{
	if(this->isFlushed)
		return;

	//TODO: truncate file
	this->outputStream->SeekTo(0);
	this->WriteContainer(*this->outputStream);

	this->isFlushed = true;
}

void ArchiveCreationFileSystem::Move(const Path &from, const Path &to)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

UniquePointer<OutputStream> ArchiveCreationFileSystem::OpenFileForWriting(const Path &path)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return UniquePointer<OutputStream>();
}

void ArchiveCreationFileSystem::RemoveDirectory(const Path &path)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}


//Protected methods
BinaryTreeMap<Path, const MemoryFile*> ArchiveCreationFileSystem::TraverseFiles() const
{
	BinaryTreeMap<Path, const MemoryFile*> result;

	ProcessDir({u8"/"}, this->root.operator->(), result);

	return result;
}


//Private methods
MemoryDirectory *ArchiveCreationFileSystem::GetDirectory(const Path &dirPath)
{
	Path rest;
	String current = dirPath.SplitOutmostPathPart(rest);
	ASSERT_EQUALS(true, current.IsEmpty());

	MemoryDirectory* dir = this->root.operator->();
	while (!rest.String().IsEmpty())
	{
		current = rest.SplitOutmostPathPart(rest);
		FileMetadataNode* child = dir->GetChild(current);
		dir = dynamic_cast<MemoryDirectory *>(child);
	}

	return dir;
}