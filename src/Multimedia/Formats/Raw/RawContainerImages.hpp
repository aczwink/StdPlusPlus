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
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "RawImageFormat.hpp"

namespace _stdxx_
{
	class PNG_Format : public RawImageFormat
	{
	public:
		//Constructor
		PNG_Format() : RawImageFormat(CodingFormatId::PNG)
		{
		}

		//Methods
		StdXX::String GetExtension() const override
		{
			return u8"png";
		}

		/*BinaryTreeSet<CodecId> GetSupportedCodecs(DataType dataType) const override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me

			return BinaryTreeSet<CodecId>();
		}*/

		float32 Matches(BufferInputStream &buffer) const override
		{
			//5.2 PNG Signature, we have one constant 8 byte signature :)

			DataReader reader(true, buffer);
			uint64 signature = reader.ReadUInt64();
			if(buffer.IsAtEnd())
				return FORMAT_MATCH_BUFFER_TOO_SMALL;

			return signature == 0x89504E470D0A1A0A;
		}
	};
}