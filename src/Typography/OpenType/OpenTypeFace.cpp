/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include "OpenTypeFace.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Containers/Strings/String.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
//Definitions
#define TAG_CMAP 0x636D6170
#define TAG_HEAD 0x68656164
#define TAG_HHEA 0x68686561
#define TAG_HMTX 0x686D7478
#define TAG_MAXP 0x6D617870
#define TAG_NAME 0x6E616D65
#define TAG_OS2 0x4F532F32

//Public methods
float32 OpenTypeFace::GetAscent() const
{
	return this->ascent * 1000.0f / this->unitsPerEm;
}

float32 OpenTypeFace::GetCapHeight() const
{
	return this->capHeight * 1000.0f / this->unitsPerEm;
}

float32 OpenTypeFace::GetDescent() const
{
	return this->descent * 1000.0f / this->unitsPerEm;
}

uint32 OpenTypeFace::GetGlyphIndex(uint32 codePoint) const
{
	const auto it = this->charMap.Find(codePoint);

	if(it != this->charMap.end())
		return (*it).value;

	return UINT32_MAX;
}

float32 OpenTypeFace::GetMaxX() const
{
	return this->xMax * 1000.0f / this->unitsPerEm;
}

float32 OpenTypeFace::GetMaxY() const
{
	return this->yMax * 1000.0f / this->unitsPerEm;
}

float32 OpenTypeFace::GetMinX() const
{
	return this->xMin * 1000.0f / this->unitsPerEm;
}

float32 OpenTypeFace::GetMinY() const
{
	return this->yMin * 1000.0f / this->unitsPerEm;
}

uint32 OpenTypeFace::GetNumberOfGlyphs() const
{
	return this->nGlyphs;
}

String OpenTypeFace::GetPostscriptName() const
{
	return this->postscriptName;
}

void OpenTypeFace::ReadRequiredTables(const BinaryTreeMap<uint32, uint32> &refTableOffsets, SeekableInputStream &refInput)
{
	//maxp table
	this->SetTableOffset(TAG_MAXP, refTableOffsets, refInput);
	this->ReadMaximumProfile(refInput);
	//cmap table
	this->SetTableOffset(TAG_CMAP, refTableOffsets, refInput);
	this->ReadCharMaps(refInput);
	//head table
	this->SetTableOffset(TAG_HEAD, refTableOffsets, refInput);
	this->ReadFontHeader(refInput);
	//hhea table
	this->SetTableOffset(TAG_HHEA, refTableOffsets, refInput);
	this->ReadHorizontalHeader(refInput);
	//hmtx table
	this->SetTableOffset(TAG_HMTX, refTableOffsets, refInput);
	this->ReadHorizontalMetrics(refInput);
	//name table
	this->SetTableOffset(TAG_NAME, refTableOffsets, refInput);
	this->ReadNames(refInput);
	//OS/2 table
	this->SetTableOffset(TAG_OS2, refTableOffsets, refInput);
	this->ReadOS2Header(refInput);
	//post table
}

//Protected methods
void OpenTypeFace::SetTableOffset(uint32 tag, const BinaryTreeMap<uint32, uint32> &refTableOffsets, SeekableInputStream &refInput)
{
	if(refTableOffsets.Contains(tag))
	{
		refInput.SeekTo(refTableOffsets[tag]);

		return;
	}

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
/*
http://www.microsoft.com/typography/otspec/cmap.htm
*/
void OpenTypeFace::ReadCharMaps(SeekableInputStream &refInput)
{
	uint16 nTables, i, platformId, encodingId;
	uint32 offset, bestOffset;
	uint64 startOffset;

	DataReader reader(true, refInput);

	enum
	{
		ENCODING_UNSUPPORTED,
		ENCODING_UCS2,
		ENCODING_UCS4
	} bestEncoding;

	bestEncoding = ENCODING_UNSUPPORTED;
	startOffset = refInput.QueryCurrentOffset();

	refInput.Skip(2); //version
	nTables = reader.ReadUInt16();

	for(i = 0; i < nTables; i++)
	{
		platformId = reader.ReadUInt16();
		encodingId = reader.ReadUInt16();
		offset = reader.ReadUInt32();

		if(platformId == 3 && encodingId == 1)
		{
			bestEncoding = ENCODING_UCS2;
			bestOffset = offset;
		}
		else if(platformId == 3 && encodingId == 10) //UCS-4
		{
			bestEncoding = ENCODING_UCS4;
			bestOffset = offset;
		}
	}

	refInput.SeekTo(startOffset + bestOffset);
	switch(bestEncoding)
	{
		case ENCODING_UCS2:
		{
			uint16 format;

			format = reader.ReadUInt16();
			ASSERT_TRUE(format == 4);
			this->ReadCharMapsFormat4(refInput);
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
}

/*
http://www.microsoft.com/typography/otspec/cmap.htm
*/
void OpenTypeFace::ReadCharMapsFormat4(SeekableInputStream &refInput)
{
	uint16 length, segCount, i, nGlyphIds, j, glyph, glyphIdx;
	int16 *pIdDelta;
	uint16 *pEndCount, *pStartCount, *pIdRangeOffset, *pGlyphIds;

	DataReader reader(true, refInput);

	length = reader.ReadUInt16();
	refInput.Skip(2); //language
	segCount = reader.ReadUInt16() / 2;

	refInput.Skip(2); //searchRange
	refInput.Skip(2); //entrySelector
	refInput.Skip(2); //rangeShift

	nGlyphIds = length / 2 - segCount * 4 - 8; //4 arrays of 'segCount' length and 8 other entries
	pEndCount = (uint16 *)MemAlloc(sizeof(*pEndCount) * segCount);
	pStartCount = (uint16 *)MemAlloc(sizeof(*pStartCount) * segCount);
	pIdDelta = (int16 *)MemAlloc(sizeof(*pIdDelta) * segCount);
	pIdRangeOffset = (uint16 *)MemAlloc(sizeof(*pIdRangeOffset) * segCount);
	pGlyphIds = (uint16 *)MemAlloc(sizeof(*pGlyphIds) * nGlyphIds);

	for(i = 0; i < segCount; i++)
	{
		pEndCount[i] = reader.ReadUInt16();
	}

	refInput.Skip(2); //reservedPad

	for(i = 0; i < segCount; i++)
	{
		pStartCount[i] = reader.ReadUInt16();
	}

	for(i = 0; i < segCount; i++)
	{
		pIdDelta[i] = reader.ReadInt16();
	}

	for(i = 0; i < segCount; i++)
	{
		pIdRangeOffset[i] = reader.ReadUInt16();
	}

	for(i = 0; i < nGlyphIds; i++)
	{
		pGlyphIds[i] = reader.ReadUInt16();
	}

	for(i = 0; i < segCount; i++)
	{
		for(j = pStartCount[i]; j <= pEndCount[i] && j < 0xFFFF; j++)
		{
			if(pIdRangeOffset[i] == 0)
			{
				glyph = (j + pIdDelta[i]) & 0xFFFF;
			}
			else
			{
				glyphIdx = i + pIdRangeOffset[i] / 2 - segCount + j - pStartCount[i];
				if(glyphIdx >= nGlyphIds)
					continue;
				glyph = (pGlyphIds[glyphIdx] + pIdDelta[i]) & 0xFFFF;
			}

			this->charMap.Insert(j, glyph);
		}
	}

	MemFree(pEndCount);
	MemFree(pStartCount);
	MemFree(pIdDelta);
	MemFree(pIdRangeOffset);
	MemFree(pGlyphIds);
}

/*
http://www.microsoft.com/typography/otspec/head.htm
*/
void OpenTypeFace::ReadFontHeader(StdXX::SeekableInputStream &refInput)
{
	DataReader reader(true, refInput);

	refInput.Skip(4); //table version number
	refInput.Skip(4); //fontRevision
	refInput.Skip(4); //checkSumAdjustment
	refInput.Skip(4); //magicNumber
	refInput.Skip(2); //flags

	this->unitsPerEm = reader.ReadUInt16();

	refInput.Skip(8); //created
	refInput.Skip(8); //modified

	this->xMin = reader.ReadInt16();
	this->yMin = reader.ReadInt16();
	this->xMax = reader.ReadInt16();
	this->yMax = reader.ReadInt16();

	refInput.Skip(2); //macStyle
	refInput.Skip(2); //lowestRecPPEM
	refInput.Skip(2); //fontDirectionHint

	this->indexToLocFormat = reader.ReadInt16() != 0;

	refInput.Skip(2); //glyphDataFormat
}

/*
http://www.microsoft.com/typography/otspec/hhea.htm
*/
void OpenTypeFace::ReadHorizontalHeader(SeekableInputStream &refInput)
{
	DataReader reader(true, refInput);

	refInput.Skip(4); //table version number
	refInput.Skip(2); //ascent
	refInput.Skip(2); //descent
	refInput.Skip(2); //lineGap
	refInput.Skip(2); //advanceWidthMax
	refInput.Skip(2); //minLeftSideBearing
	refInput.Skip(2); //minRightSideBearing
	refInput.Skip(2); //xMaxExtent
	refInput.Skip(2); //caretSlopeRise
	refInput.Skip(2); //caretSlopeRun
	refInput.Skip(2); //caretOffset
	refInput.Skip(8); //reserved = 0
	refInput.Skip(2); //metricDataFormat

	this->numberOfHMetrics = reader.ReadUInt16();
}

/*
http://www.microsoft.com/typography/otspec/hmtx.htm
*/
void OpenTypeFace::ReadHorizontalMetrics(SeekableInputStream &refInput)
{
	uint16 i;
	uint16 *pHorizontalAdvanceWidths;

	DataReader reader(true, refInput);

	pHorizontalAdvanceWidths = (uint16 *)MemAlloc(sizeof(*pHorizontalAdvanceWidths) * this->numberOfHMetrics);
	for(i = 0; i < this->numberOfHMetrics; i++)
	{
		pHorizontalAdvanceWidths[i] = reader.ReadUInt16();
		refInput.Skip(2); //lsb
	}

	for(auto &refKV : this->charMap)
	{
		if(refKV.value >= this->numberOfHMetrics)
			this->SetGlyphWidth(refKV.value, pHorizontalAdvanceWidths[this->numberOfHMetrics - 1]);
		else
			this->SetGlyphWidth(refKV.value, pHorizontalAdvanceWidths[refKV.value]);
	}
	MemFree(pHorizontalAdvanceWidths);
}

void OpenTypeFace::ReadMaximumProfile(SeekableInputStream &refInput)
{
	DataReader reader(true, refInput);

	refInput.Skip(4); //version

	this->nGlyphs = reader.ReadUInt16();

	this->AllocateGlyphData();
}

/*
http://www.microsoft.com/typography/otspec/name.htm
*/
void OpenTypeFace::ReadNames(SeekableInputStream &refInput)
{
	uint16 nRecords, stringOffset, i, platformID, nameID, length, offset, j, c;
	uint64 beginOffset, currentOffset;

	TextReader refTextReader(refInput, TextCodecType::ASCII);
	DataReader dataReader(true, refInput);

	beginOffset = refInput.QueryCurrentOffset();
	refInput.Skip(2); //format
	nRecords = dataReader.ReadUInt16();
	stringOffset = dataReader.ReadUInt16();

	for(i = 0; i < nRecords; i++)
	{
		platformID = dataReader.ReadUInt16();
		refInput.Skip(2); //encodingID
		refInput.Skip(2); //languageID
		nameID = dataReader.ReadUInt16();
		length = dataReader.ReadUInt16();
		offset = dataReader.ReadUInt16();

		currentOffset = refInput.QueryCurrentOffset();
		refInput.SeekTo(beginOffset + stringOffset + offset);

		String name;
		if(platformID == 0 || platformID == 3)
		{
			//UCS-2
			for(j = 0; j < length; j += 2)
			{
				c = dataReader.ReadUInt16();
				name += c;
			}
		}
		else
		{
			for(j = 0; j < length; j++)
			{
				c = dataReader.ReadByte();
				name += c;
			}
		}

		switch(nameID)
		{
			case 6:
				this->postscriptName = name;
				break;
		}

		refInput.SeekTo(currentOffset);
	}
}

/*
http://www.microsoft.com/typography/otspec/os2.htm
*/
void OpenTypeFace::ReadOS2Header(SeekableInputStream &refInput)
{
	uint16 version;

	DataReader reader(true, refInput);

	version = reader.ReadUInt16();

	refInput.Skip(2); //xAvgCharWidth
	refInput.Skip(2); //usWeightClass
	refInput.Skip(2); //usWidthClass
	refInput.Skip(2); //fsType
	refInput.Skip(2); //ySubscriptXSize
	refInput.Skip(2); //ySubscriptYSize
	refInput.Skip(2); //ySubscriptXOffset
	refInput.Skip(2); //ySubscriptYOffset
	refInput.Skip(2); //ySuperscriptXSize
	refInput.Skip(2); //ySuperscriptYSize
	refInput.Skip(2); //ySuperscriptXOffset
	refInput.Skip(2); //ySuperscriptYOffset
	refInput.Skip(2); //yStrikeoutSize
	refInput.Skip(2); //yStrikeoutPosition
	refInput.Skip(2); //sFamilyClass
	refInput.Skip(10); //panose[10]
	refInput.Skip(4); //ulUnicodeRange1
	refInput.Skip(4); //ulUnicodeRange2
	refInput.Skip(4); //ulUnicodeRange3
	refInput.Skip(4); //ulUnicodeRange4
	refInput.Skip(4); //achVendID[4]
	refInput.Skip(2); //fsSelection
	refInput.Skip(2); //usFirstCharIndex
	refInput.Skip(2); //usLastCharIndex

	this->ascent = reader.ReadInt16();

	this->descent = reader.ReadInt16();

	refInput.Skip(2); //sTypoLineGap
	refInput.Skip(2); //usWinAscent
	refInput.Skip(2); //usWinDescent
	if(version >= 1)
	{
		refInput.Skip(4); //ulCodePageRange1
		refInput.Skip(4); //ulCodePageRange2
	}
	if(version >= 2)
	{
		refInput.Skip(2); //sxHeight

		this->capHeight = reader.ReadInt16();

		refInput.Skip(2); //usDefaultChar
		refInput.Skip(2); //usBreakChar
		refInput.Skip(2); //usMaxContext
	}
}