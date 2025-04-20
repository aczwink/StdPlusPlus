/*
 * Copyright (c) 2022-2025 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/FileFormats/CommonFileFormats.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>

//TODO: file names always start with ._
class AppleDoubleFormat : public FileFormat
{
public:
	//Public methods
	FixedArray<String> GetExtensions() const override
	{
		return {0};
	}

	FixedArray<String> GetMediaTypes() const override
	{
		return {{FileMetaData::appleDouble}};
	}

	String GetName() const override
	{
		return u8"AppleDouble";
	}

	float32 Probe(SeekableInputStream& seekableInputStream) const override
	{
		DataReader dataReader(true, seekableInputStream);

		if(dataReader.ReadUInt32() != 0x00051607) //signature
			return 0;
		if(dataReader.ReadUInt32() != 0x00020000) //version
			return 0;
		dataReader.Skip(16); //filler

		uint16 nEntries = dataReader.ReadUInt16();
		for(uint16 i = 0; i < nEntries; i++)
		{
			uint32 entryId = dataReader.ReadUInt32();
			uint32 offset = dataReader.ReadUInt32();
			uint32 length = dataReader.ReadUInt32();

			switch(entryId)
			{
				case 2: //resource fork
				case 9: //finder info
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}

		return 1;
	}
};