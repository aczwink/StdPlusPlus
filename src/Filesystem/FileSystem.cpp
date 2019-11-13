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
#include <Std++/Filesystem/FileSystem.hpp>
//Local
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Filesystem/FileSystemFormat.hpp>
#include <Std++/Streams/FileInputStream.hpp>
//Namespaces
using namespace StdXX;

//Global variables
extern DynamicArray<const FileSystemFormat *> g_fsFormats;

//Local functions
static const FileSystemFormat *FindBestFormat(const Path& path)
{
	UniquePointer<SeekableInputStream> file = new FileInputStream(path);

	const FileSystemFormat *bestFormat = nullptr;
	float32 bestScore = 0;

	for(const FileSystemFormat *const& fileSystemFormat : g_fsFormats)
	{
		float32 matchScore = fileSystemFormat->Matches(*file);
		file->SetCurrentOffset(0);

		//check unusual cases
		if(matchScore == 1)
		{
			bestFormat = fileSystemFormat;
			break;
		}
		else if(matchScore > bestScore)
		{
			bestFormat = fileSystemFormat;
			bestScore = matchScore;
		}
	}

	return bestFormat;
}

//Public methods
void FileSystem::CreateDirectoryTree(const Path &directoryPath)
{
	if(directoryPath.IsAbsolute())
		this->GetRoot()->CreateDirectoryTree(Path(directoryPath.GetString().SubString(1)));
	else
		this->GetRoot()->CreateDirectoryTree(directoryPath);
}

//Class functions
UniquePointer<FileSystem> FileSystem::Create(const String &id, const Path &path)
{
	for(const FileSystemFormat *const& fileSystemFormat : g_fsFormats)
	{
		if(fileSystemFormat->GetId() == id)
		{
			return fileSystemFormat->CreateFileSystem(path);
		}
	}
	return nullptr;
}

UniquePointer<FileSystem> FileSystem::LoadFromFile(const Path &p)
{
	const FileSystemFormat *bestFormat = FindBestFormat(p);
	if(bestFormat)
		return bestFormat->OpenFileSystem(p, true);
	return nullptr;
}

UniquePointer<const FileSystem> FileSystem::LoadFromFileReadOnly(const Path &path)
{
	const FileSystemFormat *bestFormat = FindBestFormat(path);
	if(bestFormat)
		return bestFormat->OpenFileSystem(path, false);
	return nullptr;
}
