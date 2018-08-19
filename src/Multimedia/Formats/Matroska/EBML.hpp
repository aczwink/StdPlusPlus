/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Streams/SeekableInputStream.hpp>

/*
 * EBML Spec:
 * http://matroska-org.github.io/libebml/specs.html
 */
namespace StdXX
{
	namespace EBML
	{
		enum class DataType
		{
			Unknown,
			ASCII_String,
			Binary,
			Date,
			Float,
			Master,
			UInt, //1 to 8 byte
			UTF8
		};

		struct Element
		{
			uint64 id;
			uint64 dataOffset;
			uint64 dataSize;
			uint8 headerSize;
			DataType dataType;

			struct
			{
				union
				{
					float64 f;
					uint64 ui;
				};
				String string;
			} data;
		};

		enum ClassId
		{
			EBML_ID_EBML = 0x1A45DFA3,
		};

		struct Header
		{
			uint64 version = 1;
			uint8 maxIdLength = 4;
			uint8 maxSizeLength = 8;
			String docType;
			uint32 docTypeVersion = 1;
		};

		//Functions
		uint64 DecodeVariableLengthInteger(uint8 &length, InputStream &inputStream);
		void ParseElementHeader(Element &element, SeekableInputStream &inputStream);
		bool ParseHeader(Header &header, SeekableInputStream &inputStream);
		void ReadElementData(Element &element, SeekableInputStream &inputStream);
	}
}