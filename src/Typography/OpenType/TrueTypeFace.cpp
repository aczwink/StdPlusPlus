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
#include "TrueTypeFace.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Math/Vector2/Vector2.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
//Definitions
#define TAG_LOCA 0x6C6F6361

//Destructor
TrueTypeFace::~TrueTypeFace()
{
	for(uint32 i = 0; i < this->GetNumberOfGlyphs(); i++)
	{
		if(this->pGlyphs[i].pPath)
			delete this->pGlyphs[i].pPath;
	}

	MemFree(this->pGlyphs);
}

//Public methods
uint16 TrueTypeFace::GetCharWidth(uint16 c) const
{
	uint32 glyphIndex = this->GetGlyphIndex(c);
	if(glyphIndex == UINT32_MAX)
		return UINT16_MAX;

	return this->pGlyphs[glyphIndex].width;
}

/*
https://www.microsoft.com/typography/otspec/loca.htm
https://www.microsoft.com/typography/otspec/glyf.htm
*/
void TrueTypeFace::ReadTrueTypeOutlineTables(const BinaryTreeMap <uint32, uint32> &refTableOffsets, SeekableInputStream &refInput)
{
	uint32 glyphIdx;
	uint32 *pOffsets;
	uint64 glyfTableOffset;

	DataReader reader(true, refInput);

	//read the loca table
	this->SetTableOffset(TAG_LOCA, refTableOffsets, refInput);
	pOffsets = (uint32 *)MemAlloc(this->GetNumberOfGlyphs() * sizeof(*pOffsets));
	if(this->indexToLocFormat)
	{
		for(glyphIdx = 0; glyphIdx < this->GetNumberOfGlyphs(); glyphIdx++)
		{
			pOffsets[glyphIdx] = reader.ReadUInt32();
		}
	}
	else
	{
		for(glyphIdx = 0; glyphIdx < this->GetNumberOfGlyphs(); glyphIdx++)
		{
			pOffsets[glyphIdx] = (uint32)reader.ReadUInt16() * 2;
		}
	}

	//read the glyf table
	this->SetTableOffset(OPENTYPE_TRUETYPE_TABLETAG_GLYF, refTableOffsets, refInput);
	glyfTableOffset = refInput.QueryCurrentOffset();

	for(glyphIdx = 0; glyphIdx < this->GetNumberOfGlyphs(); glyphIdx++)
	{
		refInput.SeekTo(glyfTableOffset + pOffsets[glyphIdx]);
		this->ReadGlyphOutline(glyphIdx, refInput);
	}

	//clean up
	MemFree(pOffsets);
}

/*
 * void CTrueTypeFace::RenderGlyph(uint32 glyphIndex, Rendering::AGeneric2DPathRenderer &refRenderer) const
{
	refRenderer.SetPath(*this->pGlyphs[glyphIndex].pPath);
	refRenderer.Fill();
	//refRenderer.Stroke();
}
 */

//Private methods
void TrueTypeFace::AllocateGlyphData()
{
	uint32 size;

	size = sizeof(*this->pGlyphs) * this->GetNumberOfGlyphs();
	this->pGlyphs = (STrueTypeGlyphData *)MemAlloc(size);
	MemZero(this->pGlyphs, size);
}

/*
https://www.microsoft.com/typography/otspec/glyf.htm
*/
void TrueTypeFace::ReadGlyphOutline(uint16 glyphIndex, SeekableInputStream &refInput)
{
	uint8 flag, length;
	int16 nContours, nInstructions, lastValue, deltaVector;
	uint16 i, nPoints, j;
	uint8 *pFlags;
	int16 *pXCoord, *pYCoord;
	uint16 *pEndPtsOfContours;
	Math::Vector2D lastPoint;

	DataReader reader(true, refInput);

	nContours = reader.ReadInt16();

	if(nContours <= 0)
		return;

	ASSERT_TRUE(nContours >= 0); //< 0 ==> composite glyph
	refInput.Skip(2); //int16 xMin
	refInput.Skip(2); //int16 yMin
	refInput.Skip(2); //int16 xMax
	refInput.Skip(2); //int16 yMax

	//end points of contours
	pEndPtsOfContours = (uint16 *)MemAlloc(sizeof(*pEndPtsOfContours) * nContours);
	for(i = 0; i < nContours; i++)
	{
		pEndPtsOfContours[i] = reader.ReadUInt16();
	}
	nPoints = pEndPtsOfContours[nContours - 1] + 1;

	//instructions
	nInstructions = reader.ReadUInt16();
	refInput.Skip(nInstructions); //uint8[] instructions

	//flags
	pFlags = (uint8 *)MemAlloc(sizeof(*pFlags) * nPoints);
	for(i = 0; i < nPoints;)
	{
		flag = reader.ReadByte();
		pFlags[i++] = flag;

		if(flag & 8) //repeat
		{
			length = reader.ReadByte();
			while(length-- && i < nPoints)
				pFlags[i++] = flag;
		}
	}

	//x coordinates
	pXCoord = (int16 *)MemAlloc(sizeof(*pXCoord) * nPoints);
	lastValue = 0;
	for(i = 0; i < nPoints; i++)
	{
		pXCoord[i] = lastValue;

		if(pFlags[i] & 2) //short vector flag
		{
			deltaVector = reader.ReadByte();
			if(!(pFlags[i] & 0x10)) //negative flag
				deltaVector = -deltaVector;

			pXCoord[i] += deltaVector;
		}
		else if(!(pFlags[i] & 0x10)) //16bit signed delta vector
			pXCoord[i] += reader.ReadInt16();

		lastValue = pXCoord[i];
	}

	//y coordinates
	pYCoord = (int16 *)MemAlloc(sizeof(*pYCoord) * nPoints);
	lastValue = 0;
	for(i = 0; i < nPoints; i++)
	{
		pYCoord[i] = lastValue;

		if(pFlags[i] & 4) //short vector flag
		{
			deltaVector = reader.ReadByte();
			if(!(pFlags[i] & 0x20)) //negative flag
				deltaVector = -deltaVector;

			pYCoord[i] += deltaVector;
		}
		else if(!(pFlags[i] & 0x20)) //16bit signed delta vector
			pYCoord[i] += reader.ReadInt16();

		lastValue = pYCoord[i];
	}

	//create path
	Rendering::VectorPath *& refpPath = this->pGlyphs[glyphIndex].pPath;
	refpPath = new Rendering::VectorPath;
	j = 0;
	for(i = 0; i < nContours; i++)
	{
		//ASSERT(pFlags[j] & 1); //on curve bit must be set
		lastPoint = Math::Vector2D(pXCoord[j], pYCoord[j]);
		j++;

		refpPath->MoveTo(lastPoint);

		for(; j < pEndPtsOfContours[i]; j++)
		{
			if(pFlags[j] & 1) //on curve
			{
				lastPoint = Math::Vector2D(pXCoord[j], pYCoord[j]);
				refpPath->LineTo(lastPoint);
			}
			else
			{
				Math::Vector2D controlPoint;

				//quadratic bezier

				controlPoint = Math::Vector2D(pXCoord[j], pYCoord[j]);
				j++;

				if(pFlags[j] & 1)
				{
					//a normal bezier
					lastPoint = Math::Vector2D(pXCoord[j], pYCoord[j]);
				}
				else
				{
					//end point is omitted and is midway between last and current
					lastPoint = (Math::Vector2D(pXCoord[j], pYCoord[j]) + controlPoint) / 2;
					j--; //we need to reuse this control point; its gonna be the control point of the next segment
				}

				refpPath->BezierTo(controlPoint, lastPoint);
			}
		}

		refpPath->Close();
	}

	MemFree(pEndPtsOfContours);
	MemFree(pFlags);
	MemFree(pXCoord);
	MemFree(pYCoord);
}

void TrueTypeFace::SetGlyphWidth(uint16 glyphIndex, uint16 width)
{
	this->pGlyphs[glyphIndex].width = width;
}