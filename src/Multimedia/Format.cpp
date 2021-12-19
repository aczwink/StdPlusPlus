/*
 * Copyright (c) 2017-2019,2021 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include <Std++/Multimedia/Format.hpp>
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Multimedia/FormatRegistry.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;
//Definitions
#define DETECTIONBUFFER_MINSIZE 64
#define DETECTIONBUFFER_MAXSIZE 64u

//Local functions
static void SkipID3(SeekableInputStream &inputStream)
{
	byte id3Identifier[3];
	uint32 totalTagSize;

	inputStream.ReadBytes(id3Identifier, 3);
	if(MemCmp(id3Identifier, "ID3", 3) == 0)
	{
		inputStream.Skip(3);

		DataReader reader(true, inputStream);
		totalTagSize = ((reader.ReadByte() & 0x7F) << 21) | ((reader.ReadByte() & 0x7F) << 14) | ((reader.ReadByte() & 0x7F) << 7) | (reader.ReadByte() & 0x7F);

		inputStream.Skip(totalTagSize);
	}
	else
	{
		inputStream.Rewind(3);
	}
}

//Class constants
const float32 Format::FORMAT_MATCH_BUFFER_TOO_SMALL = -1;

//Class functions
const Format *Format::Find(SeekableInputStream &inputStream)
{
	bool resize;
	byte *pDetectionBuffer;
	uint32 detectionBufferSize, nReadBytes;
	uint64 currentOffset;
	float32 matchScore, bestScore;
	const Format *pBestFormat;

	resize = true;
	pDetectionBuffer = NULL;
	currentOffset = inputStream.QueryCurrentOffset();
	detectionBufferSize = DETECTIONBUFFER_MINSIZE;
	bestScore = 0;
	pBestFormat = NULL;

	SkipID3(inputStream);

	while(resize)
	{
		pDetectionBuffer = (byte *)MemRealloc(pDetectionBuffer, detectionBufferSize);
		inputStream.SeekTo(currentOffset);
		nReadBytes = inputStream.ReadBytes(pDetectionBuffer, detectionBufferSize);
		if(detectionBufferSize != nReadBytes)
			break; //end of input reached... we can't do anything anymore
		resize = false;

		for(const UniquePointer<Format>& format : FormatRegistry::Instance().ContainerFormats())
		{
			BufferInputStream detectionBuffer(pDetectionBuffer, detectionBufferSize);

			matchScore = format->Matches(detectionBuffer);

			//check unusual cases
			if(matchScore == FORMAT_MATCH_BUFFER_TOO_SMALL)
			{
				if(detectionBufferSize < DETECTIONBUFFER_MAXSIZE)
					resize = true;
			}
			else if(matchScore == 1)
			{
				pBestFormat = format.operator->();
				goto end;
			}
			else if(matchScore > bestScore)
			{
				pBestFormat = format.operator->();
				bestScore = matchScore;
			}
		}

		if(resize)
		{
			detectionBufferSize = Math::Min(detectionBufferSize * 2, DETECTIONBUFFER_MAXSIZE);
		}
	}

	end:;
	MemFree(pDetectionBuffer);
	inputStream.SeekTo(currentOffset);

	return pBestFormat;
}

const Format *Format::FindByExtension(const String &refExtension)
{
	for(const UniquePointer<Format>& format : FormatRegistry::Instance().ContainerFormats())
	{
		if(format->GetExtension() == refExtension)
			return format.operator->();
	}

	return nullptr;
}