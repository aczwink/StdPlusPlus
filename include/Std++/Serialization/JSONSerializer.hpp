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
#include <Std++/Streams/OutputStream.hpp>
#include "Binding.hpp"

namespace StdXX::Serialization
{
	class JSONSerializer
	{
	public:
		//Constructor
		inline JSONSerializer(OutputStream& outputStream) : textWriter(outputStream, TextCodecType::UTF8)
		{
		}

		//Operators
		inline JSONSerializer& operator<<(const Binding<int32>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<String>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		template <typename T>
		inline JSONSerializer& operator&(const Binding<T>& binding)
		{
			return *this << binding;
		}

		template <typename T>
		inline JSONSerializer& operator<<(const Binding<T>& binding)
		{
			this->objectStack.Push(CommonFileFormats::JsonValue::Object());
			Archive(*this, binding.value);
			const CommonFileFormats::JsonValue& object = this->objectStack.Pop();
			this->objectStack.Last()[binding.name] = object;
			return *this;
		}

		template <typename T>
		inline JSONSerializer& operator<<(const T& value)
		{
			this->objectStack.Push(CommonFileFormats::JsonValue::Object());
			Archive(*this, const_cast<T&>(value));
			const CommonFileFormats::JsonValue& object = this->objectStack.Pop();
			this->textWriter.WriteString(object.Dump());
			return *this;
		}

	private:
		//Members
		TextWriter textWriter;
		DynamicArray<CommonFileFormats::JsonValue> objectStack;
	};
}