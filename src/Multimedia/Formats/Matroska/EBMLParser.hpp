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
#include <Std++/Streams/InputStream.hpp>
#include "EBML.hpp"

//Spec: http://matroska-org.github.io/libebml/specs.html
namespace StdXX
{
	enum class EBMLType
	{
		ASCII_String,
		Binary,
		Date,
		Float,
		Master,
		UInt, //1 to 8 byte
		UTF8
	};

	struct EBMLElementInfo
	{
		EBMLType type;
	};

	class EBMLParser
	{
	public:
		//Constructor
		EBMLParser(InputStream &inputStream);

	private:
		//Members
		InputStream &inputStream;
		bool ebmlElementFound;

		//Methods
		uint64 DecodeVariableLengthInteger(uint8 &refLength);
		void ParseEBMLHeader(EBMLHeader &header);
		uint64 ParseElement();
		uint8 ParseElementHeader(uint64 &id, uint64 &size);

		//Overrideable
		virtual bool BeginParseChildren(uint64 id);
		virtual void EndParseChildren(uint64 id);
		virtual bool GetElementInfo(uint64 id, EBMLElementInfo &elementInfo) const;
		virtual void ParseBinary(uint64 id, uint64 size);
		virtual void ParseFloat(uint64 id, float64 value);
		virtual void ParseString(uint64 id, const String &string);
		virtual void ParseUInt(uint64 id, uint64 value);

		//Inline
		inline uint64 PutLength(uint64 uncoded, uint8 length)
		{
			return uncoded | (1ULL << uint64((length - 1) * 8 + (8 - length)));
		}
	};
}