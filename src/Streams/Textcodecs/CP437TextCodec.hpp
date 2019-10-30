/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;

namespace _stdxx_
{
	class CP437TextCodec : public TextCodec
	{
	public:
		//Methods
		uint32 ReadCodePoint(InputStream &inputStream, uint8& nBytesRead) const override
		{
			uint8 b;
			nBytesRead = inputStream.ReadBytes(&b, 1);

			if(Math::IsValueInInterval(b, 32_u8, 0x7E_u8))
				return b;

			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		void WriteCodePoint(uint32 codePoint, OutputStream &outputStream) const override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	};
}