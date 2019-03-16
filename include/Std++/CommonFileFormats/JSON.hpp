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
//Local
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Map/Map.hpp>

namespace StdXX
{
	namespace CommonFileFormats
	{
		class JSONValue
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

		public:
			//Constructors
			inline JSONValue() : type(JSONType::Null)
			{
			}

			inline JSONValue(bool b) : type(JSONType::Boolean)
			{
				this->value.boolean = b;
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

			JSONValue &operator[](const String& key)
			{
				ASSERT(this->type == JSONType::Object, u8"Can only assign to objects");
				return this->value.object->operator[](key);
			}

			//Methods
			String Dump() const;

			//Inline
			inline void Push(JSONValue&& value)
			{
				ASSERT(this->type == JSONType::Array, u8"Can only add to arrays");
				this->value.array->Push(Move(value));
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
				value.value.object = new Map<String, JSONValue>();
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
				Map<String, JSONValue>* object;
			} value;
			String stringValue;

			//Methods
			void Release();
		};
	}
}