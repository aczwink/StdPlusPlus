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
#include <Std++/Typography/Typeface.hpp>
//Local
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include "OpenType/TrueTypeFace.hpp"
#include "OpenType/PostscriptTypeface.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Typography;

//Local functions
/*
http://www.microsoft.com/typography/otspec/otff.htm
*/
static Typeface *GetOpenTypeInstance(SeekableInputStream &refInput)
{
	uint16 nTables, i;
	uint32 tag, tableOffset;
	BinaryTreeMap<uint32, uint32> tableOffsets;

	DataReader reader(true, refInput);

	tag = reader.ReadUInt32();

	switch(tag)
	{
		case 0x10000:
		case 'OTTO':
			break;
		default:
			return nullptr;
	}

	nTables = reader.ReadUInt16();
	refInput.Skip(6); //searchRange, entrySelector, rangeShift

	for(i = 0; i < nTables; i++)
	{
		tag = reader.ReadUInt32();
		refInput.Skip(4); //checksum
		tableOffset = reader.ReadUInt32();
		refInput.Skip(4); //length

		tableOffsets.Insert(tag, tableOffset);
	}

	if(tableOffsets.Contains(OPENTYPE_TRUETYPE_TABLETAG_GLYF))
	{
		TrueTypeFace *pTypeface = new TrueTypeFace;

		pTypeface->ReadRequiredTables(tableOffsets, refInput);
		pTypeface->ReadTrueTypeOutlineTables(tableOffsets, refInput);

		return pTypeface;
	}
	else if(tableOffsets.Contains(OPENTYPE_POSTSCRIPT_TABLETAG_CFF))
	{
		PostscriptTypeface *pTypeface = new PostscriptTypeface;

		pTypeface->ReadRequiredTables(tableOffsets, refInput);
		pTypeface->ReadPostscriptOutlineTables(tableOffsets, refInput);

		return pTypeface;
	}

	return nullptr;
}

//Class functions
Typeface *Typeface::GetInstance(SeekableInputStream &refInput)
{
	uint64 currentOffset = refInput.QueryCurrentOffset();

	//check if it is OpenType
	Typeface *pTypeface = GetOpenTypeInstance(refInput);
	if(pTypeface)
		return pTypeface;
	refInput.SeekTo(currentOffset);

	//either not a font or format not supported

	return nullptr;
}