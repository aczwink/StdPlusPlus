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
#include "PostscriptTypeface.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Containers/Array/DynamicArray.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
uint16 PostscriptTypeface::GetCharWidth(uint16 c) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

uint32 PostscriptTypeface::GetGlyphIndex(uint32 codePoint) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void PostscriptTypeface::ReadPostscriptOutlineTables(const BinaryTreeMap<uint32, uint32> &refTableOffsets, SeekableInputStream &refInput)
{
	this->SetTableOffset(OPENTYPE_POSTSCRIPT_TABLETAG_CFF, refTableOffsets, refInput);

	//6. Header
	refInput.Skip(1); //major
	refInput.Skip(1); //minor
	refInput.Skip(1); //header size
	refInput.Skip(1); //offsize

	//7. Name INDEX
	this->ReadIndex(refInput);

	//TODO:
	//Top DICT INDEX
	//String INDEX
	//Global Subr INDEX
	//Encodings
	//Charsets
	//FDSelect					CIDFonts only
	//CharStrings INDEX			per - font
	//Font DICT INDEX			per-font, CIDFonts only
	//Private DICT				per-font
	//Local Subr INDEX			per-font or per-Private DICT for CIDFonts
	//Copyright and Trademark Notices
}

//Private methods
void PostscriptTypeface::AllocateGlyphData()
{
}

void PostscriptTypeface::ReadIndex(SeekableInputStream &refInput)
{
	uint8 offsetSize;
	uint16 count, i;
	DynamicArray<uint32> offsets;

	DataReader dataReader(true, refInput);

	count = dataReader.ReadUInt16();
	if(count == 0)
		return;
	offsetSize = dataReader.ReadByte();

	//read offsets
	for(i = 0; i < count + 1; i++)
		offsets.Push(this->ReadOffset(offsetSize, refInput));

	//skip data
	refInput.Skip(offsets[count] - 1);
}

uint32 PostscriptTypeface::ReadOffset(uint8 offsetSize, InputStream &refInput)
{
	DataReader dataReader(true, refInput);

	switch(offsetSize)
	{
		case 1:
			return dataReader.ReadByte();
		case 2:
			return dataReader.ReadUInt16();
		case 4:
			return dataReader.ReadUInt32();
	}

	NOT_IMPLEMENTED_ERROR;
	return -1;
}

void PostscriptTypeface::SetGlyphWidth(uint16 glyphIndex, uint16 width)
{
}