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
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Destructor
JSONValue::~JSONValue()
{
	this->Release();
}

//Operators
JSONValue &JSONValue::operator=(const JSONValue &rhs)
{
	this->type = rhs.type;
	this->value = rhs.value;
	this->stringValue = rhs.stringValue;

	switch(this->type)
	{
		case JSONType::Array:
			this->value.array = new DynamicArray<JSONValue>(*rhs.value.array);
			break;
		case JSONType::Object:
			this->value.object = new Map<String, JSONValue>(*rhs.value.object);
			break;
	}

	return *this;
}

JSONValue &JSONValue::operator=(JSONValue &&rhs)
{
	this->type = rhs.type;
	this->value = rhs.value;
	this->stringValue = Move(rhs.stringValue);

	rhs.type = JSONType::Null;
	rhs.value.array = nullptr;
	rhs.value.object = nullptr;

	return *this;
}

//Public methods
String JSONValue::Dump() const
{
	switch(this->type)
	{
		case JSONType::Array:
		{
			bool putComma = false;
			String tmp;

			for(const JSONValue& v : *this->value.array)
			{
				if(putComma)
					tmp += u8",";
				tmp += v.Dump();
				putComma = true;
			}
			return u8"[" + tmp + u8"]";
		}
		case JSONType ::Boolean:
			if(this->value.boolean)
				return u8"true";
			return u8"false";
		case JSONType::Null:
			return u8"null";
		case JSONType::Number:
			return String::Number(this->value.number);
		case JSONType::Object:
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
		case JSONType::String:
			return u8"\"" + this->stringValue + u8"\"";
	}

	NOT_IMPLEMENTED_ERROR;
	return String();
}

//Private methods
void JSONValue::Release()
{
	switch(this->type)
	{
		case JSONType::Array:
			delete this->value.array;
			break;
		case JSONType::Object:
			delete this->value.object;
			break;
	}
	this->stringValue.Release();
	this->type = JSONType::Null;
}