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
#pragma once
//Local
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>

namespace StdXX
{
	namespace CommonFileFormats
	{
		enum class JsonType
		{
			Null,
			Array,
			Boolean,
			Number,
			Object,
			String
		};

		class JsonValue
		{
		public:
			//Constructors
			inline JsonValue() : type(JsonType::Null)
			{
			}

			inline JsonValue(bool b) : type(JsonType::Boolean)
			{
				this->value.boolean = b;
			}

			inline JsonValue(uint32 number) : type(JsonType::Number)
			{
				this->value.number = number;
			}

			inline JsonValue(uint64 number) : type(JsonType::Number)
			{
				this->value.number = number;
			}

			inline JsonValue(float64 number) : type(JsonType::Number)
			{
				this->value.number = number;
			}

			inline JsonValue(const String& string) : type(JsonType::String), stringValue(string)
			{
			}

			inline JsonValue(const JsonValue& other) : type(JsonType::Null)
			{
				*this = other;
			}

			inline JsonValue(JsonValue&& other) : type(JsonType::Null)
			{
				*this = Move(other);
			}

			//Destructor
			~JsonValue();

			//Operators
			JsonValue& operator=(const JsonValue& rhs);
			JsonValue& operator=(JsonValue&& rhs);

			inline JsonValue &operator[](const String& key)
			{
				ASSERT(this->type == JsonType::Object, u8"Can only assign to objects");
				return this->value.object->operator[](key);
			}

			//Logical operators
			bool operator==(const JsonValue& rhs) const;

			inline bool operator!=(const JsonValue& rhs) const
			{
				return !(*this == rhs);
			}

			//Methods
			String Dump() const;

			//Inline
			inline const Map<String, JsonValue>& MapValue() const
			{
				ASSERT(this->type == JsonType::Object, u8"Only objects have maps.");
				return *this->value.object;
			}

			inline float64 NumberValue() const
			{
				ASSERT(this->type == JsonType::Number, u8"This is not a number.");
				return this->value.number;
			}

			inline void Push(JsonValue&& value)
			{
				ASSERT(this->type == JsonType::Array, u8"Can only add to arrays");
				this->value.array->Push(Move(value));
			}

			inline const String& StringValue() const
			{
				ASSERT(this->type == JsonType::String, u8"This is not a string.");
				return this->stringValue;
			}

			inline JsonType Type() const
			{
				return this->type;
			}

			//Functions
			static JsonValue Array()
			{
				JsonValue value;
				value.type = JsonType::Array;
				value.value.array = new DynamicArray<JsonValue>();
				return value;
			}

			static JsonValue Object()
			{
				JsonValue value;
				value.type = JsonType::Object;
				value.value.object = new Map<String, JsonValue>();
				return value;
			}

		private:
			//Members
			JsonType type;
			union
			{
				bool boolean;
				float64 number;
				DynamicArray<JsonValue>* array;
				Map<String, JsonValue>* object;
			} value;
			String stringValue;

			//Methods
			void Release();
		};

		//Functions
		/**
		 * Parses human readable JSON data.
		 * This extends JSON with trailing commas for objects and arrays and single and multi-line comments as many
		 * others such as JSON5 and Hjson propose.
		 * @param textReader
		 * @return
		 */
		JsonValue ParseHumanReadableJson(TextReader &textReader);
	}
}