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
#include <Std++/Filesystem/ContainerFileSystem.hpp>
//Local
#include <Std++/Filesystem/ContainerDirectory.hpp>
#include <Std++/Filesystem/OSFileSystem.hpp>
#include <Std++/Filesystem/DirectoryWalker.hpp>
//Namespaces
using namespace StdXX;

//Constructor
ContainerFileSystem::ContainerFileSystem(const FileSystemFormat *format, const Path &fileSystemPath) : FileSystem(format),
																									   fileSystemPath(fileSystemPath),
																									   containerInputStream((OSFileSystem::GetInstance().Exists(fileSystemPath)) ? (new FileInputStream(fileSystemPath)) : nullptr),
																									   root(new ContainerDirectory(this))
{
	this->isFlushed = !this->containerInputStream.IsNull();
}

//Public methods
UniquePointer<OutputStream> ContainerFileSystem::CreateFile(const Path &filePath)
{
	return this->GetDirectory(filePath.GetParent())->CreateFile(filePath.GetName());
}

bool ContainerFileSystem::Exists(const Path &path) const
{
	return this->root->Exists(path);
}

AutoPointer<FileSystemNode> ContainerFileSystem::GetNode(const Path &path)
{
	if(path.IsRoot())
		return this->root;

	Path leftPath = path;
	if(leftPath.IsAbsolute()) //skip root slash
		leftPath = leftPath.GetString().SubString(1);
	AutoPointer<FileSystemNode> node = this->root;
	while(true)
	{
		Path remaining;
		String name = leftPath.SplitOutmostPathPart(remaining);
		leftPath = remaining;

		ASSERT(node->GetType() == FileSystemNodeType::Directory, u8"Only directories can have children");
		node = node.Cast<Directory>()->GetChild(name);

		if(leftPath.GetString().IsEmpty())
			break;
	}

	return node;
}

AutoPointer<const FileSystemNode> ContainerFileSystem::GetNode(const Path &path) const
{
	if(path.IsRoot())
		return this->root;

	Path leftPath = path;
	if(leftPath.IsAbsolute()) //skip root slash
		leftPath = leftPath.GetString().SubString(1);
	AutoPointer<const FileSystemNode> node = this->root;
	while(true)
	{
		Path remaining;
		String name = leftPath.SplitOutmostPathPart(remaining);
		leftPath = remaining;

		ASSERT(node->GetType() == FileSystemNodeType::Directory, u8"Only directories can have children");
		node = node.Cast<const Directory>()->GetChild(name);

		if(leftPath.GetString().IsEmpty())
			break;
	}

	return node;
}

AutoPointer<Directory> ContainerFileSystem::GetRoot()
{
	return this->root;
}

uint64 ContainerFileSystem::GetSize() const
{
	return this->containerInputStream->GetSize();
}

//Protected methods
void ContainerFileSystem::AddSourceFile(const Path& filePath, ContainerFile *file)
{
	bool flushStatus = this->isFlushed;

	Path directoryPath = filePath.GetParent();
	this->CreateDirectoryTree(directoryPath);
	AutoPointer<Directory> dir = this->GetDirectory(directoryPath);
	AutoPointer<ContainerDirectory> containerDirectory = dir.Cast<ContainerDirectory>();

	containerDirectory->AddSourceFile(filePath.GetName(), file);

	this->isFlushed = flushStatus; //the flushed status is not affected by source files
}

UniquePointer<FileOutputStream> ContainerFileSystem::OpenTempContainer()
{
	Path p = this->fileSystemPath + Path(u8".stdpptmp");
	ASSERT(!OSFileSystem::GetInstance().Exists(p), u8"If you see this, report to StdXX please");

	return new FileOutputStream(p);
}

void ContainerFileSystem::SwapWithTempContainer(UniquePointer<FileOutputStream> &tempContainer)
{
	if(!this->containerInputStream.IsNull())
		this->containerInputStream = nullptr; //close file

	Path tempPath = tempContainer->GetPath();
	tempContainer = nullptr; //close temp file

	OSFileSystem::GetInstance().Move(tempPath, this->fileSystemPath);

	//TODO: we now have to reopen this->containerInputStream and read again all offsets...
	//TODO: also all files that have buffers must be closed now... i.e. the buffers must be deleted
}

void ContainerFileSystem::Move(const Path &from, const Path &to)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}