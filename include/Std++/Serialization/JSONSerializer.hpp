/*
 * Copyright (c) 2020-2025 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/CommonFileFormats/YAML.hpp>
#include <Std++/Streams/OutputStream.hpp>
#include "Binding.hpp"
#include "General.hpp"

namespace StdXX::Serialization
{
	class JSONSerializer
	{
	public:
		//Constructor
		inline JSONSerializer(OutputStream& outputStream, bool writeYAMLInstead = false) : textWriter(outputStream, TextCodecType::UTF8), writeYAML(writeYAMLInstead)
		{
		}

		//Operators
		inline JSONSerializer& operator<<(const Binding<bool>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<float32>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<int32>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<uint32>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<String>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		inline JSONSerializer& operator<<(const Binding<const String>& binding)
		{
			this->objectStack.Last()[binding.name] = binding.value;
			return *this;
		}

		template <typename T>
		inline JSONSerializer& operator<<(const Binding<DynamicArray<T>>& binding)
		{
			CommonFileFormats::JSONValue array = CommonFileFormats::JSONValue::Array();
			for(const auto& value : binding.value)
			{
				CommonFileFormats::JSONValue element = value;
				array.Push(Move(element));
			}
			this->objectStack.Last()[binding.name] = Move(array);
			return *this;
		}

		template <typename T>
		inline JSONSerializer& operator&(const Binding<T>& binding)
		{
			return *this << binding;
		}

		template <typename T>
		inline
		Type::EnableIf_t<HasArchiveFunction<JSONSerializer, T>::value, JSONSerializer&>
		operator<<(const Binding<T>& binding)
		{
			this->EnterObject(binding.name);
			Archive(*this, binding.value);
			this->LeaveObject();
			return *this;
		}

		template <typename T>
		inline
		Type::EnableIf_t<HasArchiveFunction<JSONSerializer, T>::value, JSONSerializer&>
		operator<<(const T& value)
		{
			this->EnterObject(u8"");
			Archive(*this, const_cast<T&>(value));
			this->LeaveObject();
			return *this;
		}

		//Inline
		inline void EnterObject(const String& name)
		{
			this->currentObjectName = name;
			this->objectStack.Push(CommonFileFormats::JSONValue::Object());
		}

		inline void LeaveObject()
		{
			CommonFileFormats::JSONValue object = this->objectStack.Pop();
			if(this->objectStack.IsEmpty())
				this->Finalize(object);
			else
				this->objectStack.Last()[this->currentObjectName] = Move(object);
		}

	private:
		//Members
		bool writeYAML;
		TextWriter textWriter;
		DynamicArray<CommonFileFormats::JSONValue> objectStack;
		String currentObjectName;

		//Inline
		inline void Finalize(const CommonFileFormats::JSONValue& object)
		{
			if(this->writeYAML)
				CommonFileFormats::YAML::Write(object, this->textWriter);
			else
				this->textWriter.WriteString(object.Dump());
		}
	};
}