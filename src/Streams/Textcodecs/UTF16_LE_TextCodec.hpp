/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/TextCodec.hpp>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace StdXX;

class UTF16_LE_TextCodec : public TextCodec
{
public:
	//Methods
	uint32 ReadCodePoint(InputStream &inputStream) const override
	{
		DataReader reader(false, inputStream);

		uint16 codePoint = reader.ReadUInt16();
		if (codePoint > 0xD800)
		{
			NOT_IMPLEMENTED_ERROR; //surrogate-pair
		}

		return codePoint;
	}

	void WriteCodePoint(uint32 codePoint, OutputStream &outputStream) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}
};