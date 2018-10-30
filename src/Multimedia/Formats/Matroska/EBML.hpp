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
#pragma once
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

			//Inline
			inline bool SizeUnknown() const
			{
				return this->dataSize == Natural<uint64>::Max();
			}
		};

		enum ClassId
		{
			EBML_ID_EBML = 0x1A45DFA3,
			EBML_ID_DOCTYPE = 0x4282,
			EBML_ID_DOCTYPEVERSION = 0x4287,
			EBML_ID_DOCTYPEREADVERSION = 0x4285,
			EBML_ID_VOID = 0xEC,
			EBML_ID_CRC32 = 0xBF,
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
		uint64 DecodeVariableLengthInteger(uint8 &length, InputStream &inputStream, bool isElementSize = false);
		void ParseElementHeader(Element &element, InputStream &inputStream);
		bool ParseHeader(Header &header, SeekableInputStream &inputStream);
		void ReadElementData(Element &element, InputStream &inputStream);
	}
}