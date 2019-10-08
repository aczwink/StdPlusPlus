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
#include <Std++/CommonFileFormats/JSON.hpp>
//Local
#include "HumanReadableJsonParser.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Destructor
JsonValue::~JsonValue()
{
	this->Release();
}

//Operators
JsonValue &JsonValue::operator=(const JsonValue &rhs)
{
	this->type = rhs.type;
	this->value = rhs.value;
	this->stringValue = rhs.stringValue;

	switch(this->type)
	{
		case JsonType::Array:
			this->value.array = new DynamicArray<JsonValue>(*rhs.value.array);
			break;
		case JsonType::Object:
			this->value.object = new Map<String, JsonValue>(*rhs.value.object);
			break;
	}

	return *this;
}

JsonValue &JsonValue::operator=(JsonValue &&rhs)
{
	this->type = rhs.type;
	this->value = rhs.value;
	this->stringValue = Move(rhs.stringValue);

	rhs.type = JsonType::Null;
	rhs.value.array = nullptr;
	rhs.value.object = nullptr;

	return *this;
}

bool JsonValue::operator==(const JsonValue &rhs) const
{
	if(this->type != rhs.type)
		return false;

	switch(this->type)
	{
		case JsonType::Null:
			return true;
		case JsonType::Array:
			return *this->value.array == *rhs.value.array;
		case JsonType::Boolean:
			return this->value.boolean == rhs.value.boolean;
		case JsonType::Number:
			return this->value.number == rhs.value.number;
		case JsonType::Object:
			return *this->value.object == *rhs.value.object;
		case JsonType::String:
			return this->stringValue == rhs.stringValue;
	}

	NOT_IMPLEMENTED_ERROR;
	return false;
}

//Public methods
String JsonValue::Dump() const
{
	switch(this->type)
	{
		case JsonType::Array:
		{
			bool putComma = false;
			String tmp;

			for(const JsonValue& v : *this->value.array)
			{
				if(putComma)
					tmp += u8",";
				tmp += v.Dump();
				putComma = true;
			}
			return u8"[" + tmp + u8"]";
		}
		case JsonType ::Boolean:
			if(this->value.boolean)
				return u8"true";
			return u8"false";
		case JsonType::Null:
			return u8"null";
		case JsonType::Number:
			return String::Number(this->value.number);
		case JsonType::Object:
		{
			bool putComma = false;
			String tmp;

			for(const auto& kv : *this->value.object)
			{
				if(putComma)
					tmp += u8",";
				tmp += u8"\"" + kv.key + u8"\":";
				tmp += kv.value.Dump();
				putComma = true;
			}
			return u8"{" + tmp + u8"}";
		}
		case JsonType::String:
			return u8"\"" + this->stringValue + u8"\"";
	}

	NOT_IMPLEMENTED_ERROR;
	return String();
}

//Private methods
void JsonValue::Release()
{
	switch(this->type)
	{
		case JsonType::Array:
			delete this->value.array;
			break;
		case JsonType::Object:
			delete this->value.object;
			break;
	}
	this->stringValue.Release();
	this->type = JsonType::Null;
}

//Namespace functions
JsonValue CommonFileFormats::ParseHumanReadableJson(TextReader &textReader)
{
	_stdxx_::HumanReadableJsonParser parser(textReader);
	return parser.Parse();
}
