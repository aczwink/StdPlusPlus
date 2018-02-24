/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;
//Definitions
#define DETECTIONBUFFER_MINSIZE 17
#define DETECTIONBUFFER_MAXSIZE 17

//Global variables
DynamicArray<const Format *> g_formats;

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
	currentOffset = inputStream.GetCurrentOffset();
	detectionBufferSize = DETECTIONBUFFER_MINSIZE;
	bestScore = 0;
	pBestFormat = NULL;

	SkipID3(inputStream);

	while(resize)
	{
		pDetectionBuffer = (byte *)MemRealloc(pDetectionBuffer, detectionBufferSize);
		inputStream.SetCurrentOffset(currentOffset);
		nReadBytes = inputStream.ReadBytes(pDetectionBuffer, detectionBufferSize);
		if(detectionBufferSize != nReadBytes)
			break; //end of input reached... we can't do anything anymore
		resize = false;

		for(const Format *const& refpFormat : g_formats)
		{
			BufferInputStream detectionBuffer(pDetectionBuffer, detectionBufferSize);

			matchScore = refpFormat->Matches(detectionBuffer);

			//check unusual cases
			if(matchScore == FORMAT_MATCH_BUFFER_TOO_SMALL)
			{
				if(detectionBufferSize < DETECTIONBUFFER_MAXSIZE)
					resize = true;
			}
			else if(matchScore == 1)
			{
				pBestFormat = refpFormat;
				goto end;
			}
			else if(matchScore > bestScore)
			{
				pBestFormat = refpFormat;
				bestScore = matchScore;
			}
		}

		if(resize)
		{
			detectionBufferSize = MIN(detectionBufferSize * 2, DETECTIONBUFFER_MAXSIZE);
		}
	}

	end:;
	MemFree(pDetectionBuffer);
	inputStream.SetCurrentOffset(currentOffset);

	return pBestFormat;
}

const Format *Format::FindByExtension(const OldString &refExtension)
{
	for(const Format *const& refpFormat : g_formats)
	{
		if(refpFormat->GetExtension() == refExtension)
			return refpFormat;
	}

	return nullptr;
}

void Format::Register(Format *pFormat)
{
	g_formats.Push(pFormat);
}