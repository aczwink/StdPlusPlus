/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Format.hpp>
#include "../QuickTime/QuickTimeFormat.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class MP4_Format : public QuickTimeFormat
{
public:
	String GetExtension() const override
	{
		return u8"mp4";
	}

	String GetName() const override
	{
		return u8"MP4 (MPEG-4 Part 14)";
	}

	float32 Probe(BufferInputStream &inputStream) const override
	{
		DataReader dataReader(false, inputStream);

		dataReader.Skip(4);
		if(dataReader.ReadUInt32() == FOURCC(u8"ftyp"))
		{
			switch(dataReader.ReadUInt32())
			{
				case FOURCC(u8"isom"):
				case FOURCC(u8"M4A "):
					return 1;
			}
		}
		return 0;
	}
};