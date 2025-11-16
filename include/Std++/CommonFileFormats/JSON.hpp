/*
 * Copyright (c) 2019-2025 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>

namespace StdXX
{
	namespace CommonFileFormats
	{
		enum class JSONType
		{
			Null,
			Array,
			Boolean,
			Number,
			Object,
			String
		};

		class JSONValue
		{
		public:
			//Constructors
			inline JSONValue() : type(JSONType::Null)
			{
			}

			inline JSONValue(bool b) : type(JSONType::Boolean)
			{
				this->value.boolean = b;
			}

			inline JSONValue(int32 number) : type(JSONType::Number)
			{
				this->value.number = number;
			}

			inline JSONValue(uint32 number) : type(JSONType::Number)
			{
				this->value.number = number;
			}

			inline JSONValue(uint64 number) : type(JSONType::Number)
			{
				this->value.number = number;
			}

			inline JSONValue(float64 number) : type(JSONType::Number)
			{
				this->value.number = number;
			}

			inline JSONValue(const char* string) : type(JSONType::String), stringValue(string)
			{
			}

			inline JSONValue(const String& string) : type(JSONType::String), stringValue(string)
			{
			}

			inline JSONValue(const JSONValue& other) : type(JSONType::Null)
			{
				*this = other;
			}

			inline JSONValue(JSONValue&& other) : type(JSONType::Null)
			{
				*this = Move(other);
			}

			//Destructor
			~JSONValue();

			//Operators
			JSONValue& operator=(const JSONValue& rhs);
			JSONValue& operator=(JSONValue&& rhs);

			inline JSONValue &operator[](const String& key)
			{
				ASSERT(this->type == JSONType::Object, u8"Can only assign to objects");
				return this->value.object->operator[](key);
			}

			inline const JSONValue &operator[](const String& key) const
			{
				ASSERT(this->type == JSONType::Object, u8"Can only access objects by key");
				const BinaryTreeMap<String, JSONValue>& map = *this->value.object;
				return map[key];
			}

			//Logical operators
			bool operator==(const JSONValue& rhs) const;

			inline bool operator!=(const JSONValue& rhs) const
			{
				return !(*this == rhs);
			}

			//Properties
			inline const DynamicArray<JSONValue>& ArrayValue() const
			{
				ASSERT(this->type == JSONType::Array, u8"Only arrays have this.");
				return *this->value.array;
			}

			inline bool BoolValue() const
			{
				ASSERT(this->type == JSONType::Boolean, u8"Only bools have this.");
				return this->value.boolean;
			}

			//Methods
			String Dump() const;

			//Inline
			inline void Delete(const String& key)
			{
				ASSERT_EQUALS(this->type, JSONType::Object);
				this->value.object->Remove(key);
			}

			/**
			 * Returns the property "this[key]" if defined else "defaultValue".
			 */
			inline JSONValue Get(const String& key, const JSONValue& defaultValue) const
			{
				ASSERT(this->type == JSONType::Object, u8"Can only assign to objects");
				if(this->value.object->Contains(key))
					return this->value.object->operator[](key);
				return defaultValue;
			}

			inline const BinaryTreeMap<String, JSONValue>& MapValue() const
			{
				ASSERT(this->type == JSONType::Object, u8"Only objects have maps.");
				return *this->value.object;
			}

			inline float64 NumberValue() const
			{
				ASSERT(this->type == JSONType::Number, u8"This is not a number.");
				return this->value.number;
			}

			inline void Push(JSONValue&& value)
			{
				ASSERT(this->type == JSONType::Array, u8"Can only add to arrays");
				this->value.array->Push(Move(value));
			}

			inline const String& StringValue() const
			{
				ASSERT(this->type == JSONType::String, u8"This is not a string.");
				return this->stringValue;
			}

			inline JSONType Type() const
			{
				return this->type;
			}

			//Functions
			static JSONValue Array()
			{
				JSONValue value;
				value.type = JSONType::Array;
				value.value.array = new DynamicArray<JSONValue>();
				return value;
			}

			static JSONValue Object()
			{
				JSONValue value;
				value.type = JSONType::Object;
				value.value.object = new BinaryTreeMap<String, JSONValue>();
				return value;
			}

		private:
			//Members
			JSONType type;
			union
			{
				bool boolean;
				float64 number;
				DynamicArray<JSONValue>* array;
				BinaryTreeMap<String, JSONValue>* object;
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
		JSONValue ParseHumanReadableJson(TextReader &textReader);
		JSONValue ParseJson(TextReader& textReader);
	}
}