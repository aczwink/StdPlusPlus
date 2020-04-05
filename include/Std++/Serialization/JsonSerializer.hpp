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

namespace StdXX::Serialization
{
	class JsonSerializer
	{
	public:
		//Constructor
		inline JsonSerializer(OutputStream& outputStream) : textWriter(outputStream, TextCodecType::UTF8)
		{
		}

		//Operators
		template <typename T>
		inline JsonSerializer& operator&(const Binding<T>& binding)
		{
			return *this << binding;
		}

		template <typename T>
		inline JsonSerializer& operator<<(const Binding<T>& binding)
		{
			this->objectStack.Push(CommonFileFormats::JsonValue::Object());
			*this << binding.value;
			this->textWriter.WriteString(this->objectStack.Pop().Dump());
			return *this;
		}

	private:
		//Members
		TextWriter textWriter;
		DynamicArray<CommonFileFormats::JsonValue> objectStack;
	};
}