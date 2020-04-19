/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "Binding.hpp"

namespace StdXX::Serialization
{
	class JSONDeserializer
	{
	public:
		inline JSONDeserializer(InputStream& inputStream, bool humanReadable = false) : inputStream(inputStream)
		{
			TextReader textReader(inputStream, TextCodecType::UTF8);
			if(humanReadable)
				this->root = CommonFileFormats::ParseHumanReadableJson(textReader);
			else
				this->root = CommonFileFormats::ParseJson(textReader);
		}

		//Operators
		template <typename T>
		inline JSONDeserializer& operator&(const Binding<T>& binding)
		{
			return *this >> binding;
		}

		JSONDeserializer& operator>>(const Binding<int32>& binding)
		{
			binding.value = this->DeserializeNumber(binding.name);
			return *this;
		}

		JSONDeserializer& operator>>(const Binding<uint8>& binding)
		{
			binding.value = this->DeserializeNumber(binding.name);
			return *this;
		}

		JSONDeserializer& operator>>(const Binding<uint16>& binding)
		{
			binding.value = this->DeserializeNumber(binding.name);
			return *this;
		}

		JSONDeserializer& operator>>(const Binding<uint32>& binding)
		{
			binding.value = this->DeserializeNumber(binding.name);
			return *this;
		}

		JSONDeserializer& operator>>(const Binding<uint64>& binding)
		{
			binding.value = this->DeserializeNumber(binding.name);
			return *this;
		}

		JSONDeserializer& operator>>(const Binding<String>& binding)
		{
			CommonFileFormats::JsonValue& object = *this->objectStack.Last();
			binding.value = object[binding.name].StringValue();
			object.Delete(binding.name);
			return *this;
		}

		template <typename T>
		JSONDeserializer& operator>>(const Binding<T>& binding)
		{
			CommonFileFormats::JsonValue& parent = *this->objectStack.Last();
			this->objectStack.Push(&parent[binding.name]);
			Archive(*this, binding.value);
			this->objectStack.Pop();
			parent.Delete(binding.name);
			return *this;
		}

		template <typename T>
		inline JSONDeserializer& operator>>(T& value)
		{
			this->objectStack.Push(&this->root);
			Archive(*this, value);
			this->objectStack.Pop();
			return *this;
		}

	private:
		//Members
		InputStream& inputStream;
		CommonFileFormats::JsonValue root;
		DynamicArray<CommonFileFormats::JsonValue*> objectStack;

		//Inline
		inline float64 DeserializeNumber(const String& propertyName)
		{
			CommonFileFormats::JsonValue& object = *this->objectStack.Last();
			const CommonFileFormats::JsonValue& constObj = object;
			float64 result = constObj[propertyName].NumberValue();
			object.Delete(propertyName);

			return result;
		}
	};
}