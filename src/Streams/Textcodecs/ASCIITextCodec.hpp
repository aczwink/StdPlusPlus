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
//Namespaces
using namespace StdPlusPlus;

class ASCIITextCodec : public TextCodec
{
public:
	//Methods
	uint32 ReadCodePoint(InputStream &inputStream) const override
	{
		byte b;
		inputStream.ReadBytes(&b, 1);
		ASSERT(b <= 0x7F, u8"Invalid ASCII value");

		return b; //ascii directly maps to unicode
	}

	void WriteCodePoint(uint32 codePoint, OutputStream &outputStream) const override
	{
		ASSERT(codePoint <= 0x7F, u8"Invalid ASCII value");
		byte b = static_cast<byte>(codePoint); //ascii directly maps to unicode
		outputStream.WriteBytes(&b, 1);
	}
};