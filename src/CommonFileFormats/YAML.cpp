/*
 * Copyright (c) 2025 Amir Czwink (amir130@hotmail.de)
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
//Corresponding header
#include <Std++/CommonFileFormats/YAML.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Local functions
static void WriteIndentation(uint8 indentation, TextWriter& textWriter)
{
	for(uint8 i = 0; i < indentation; i++)
		textWriter.WriteString(u8"  ");
}

static void WriteValue(const JSONValue& value, TextWriter& textWriter, uint8 indentation)
{
	switch(value.Type())
	{
		case JSONType::Null:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
		case JSONType::Array:
		{
			bool needComma = false;

			textWriter.WriteString(u8"[");
			for(const auto& element : value.ArrayValue())
			{
				if(needComma)
					textWriter.WriteString(u8", ");
				WriteValue(element, textWriter, indentation);
				needComma = true;
			}
			textWriter.WriteString(u8"]");
		}
		break;
		case JSONType::Boolean:
			textWriter << (value.BoolValue() ? u8"true" : u8"false");
			break;
		case JSONType::Number:
			textWriter << value.NumberValue();
			break;
		case JSONType::Object:
			for(const auto& kv : value.MapValue())
			{
				WriteIndentation(indentation, textWriter);
				textWriter.WriteString(kv.key);
				textWriter.WriteString(u8": ");
				WriteValue(kv.value, textWriter, indentation + 1_u8);
				textWriter.WriteString(u8"\n");
			}
			break;
		case JSONType::String:
			textWriter << value.StringValue();
			break;
	}
}

//Namespace functions
void YAML::Write(const JSONValue& value, TextWriter& textWriter)
{
	WriteValue(value, textWriter, 0);
}