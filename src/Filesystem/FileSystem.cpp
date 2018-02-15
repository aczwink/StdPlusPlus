/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Filesystem/FileSystem.hpp>
//Local
#include <ACStdLib/Containers/Array/DynamicArray.hpp>
#include <ACStdLib/Filesystem/FileSystemFormat.hpp>
#include <ACStdLib/Streams/FileInputStream.hpp>
//Namespaces
using namespace ACStdLib;

//Public methods
void FileSystem::CreateDirectoryTree(const Path &directoryPath)
{
	if(directoryPath.IsAbsolute())
		this->GetRoot()->CreateDirectoryTree(Path(directoryPath.GetString().SubString(1)));
	else
		this->GetRoot()->CreateDirectoryTree(directoryPath);
}

//Class functions
extern DynamicArray<const FileSystemFormat *> g_fsFormats;
UniquePointer<FileSystem> FileSystem::LoadFromFile(const Path &p)
{
	UniquePointer<SeekableInputStream> file = new FileInputStream(p);

	const FileSystemFormat *bestFormat = nullptr;
	float32 bestScore = 0;

	for(const FileSystemFormat *const& fileSystemFormat : g_fsFormats)
	{
		float32 matchScore = fileSystemFormat->Matches(*file);

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

	if(bestFormat)
	{
		file->SetCurrentOffset(0);
		return bestFormat->CreateFileSystem(Forward(file));
	}

	return nullptr;
}