/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
/*//Class header
#include <Std++/FileSystem/Format.hpp>
//Local
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Streams/FileInputStream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Global variables
DynamicArray<const Format *> g_fsFormats;

//Class functions
const Format* Format::FindBestFormat(const Path& path)
{
	UniquePointer<SeekableInputStream> file = new FileInputStream(path);

	const Format *bestFormat = nullptr;
	float32 bestScore = 0;

	for(const Format *const& fileSystemFormat : g_fsFormats)
	{
		float32 matchScore = fileSystemFormat->Matches(*file);
		file->SeekTo(0);

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

const Format *Format::GetFormatById(const String &id)
{
	for(const Format *const& fileSystemFormat : g_fsFormats)
	{
		if(fileSystemFormat->GetId() == id)
		{
			return fileSystemFormat;
		}
	}
	return nullptr;
}

void Format::Register(const Format *fileSystemFormat)
{
	g_fsFormats.Push(fileSystemFormat);
}*/