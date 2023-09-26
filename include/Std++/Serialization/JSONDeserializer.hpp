/*
 * Copyright (c) 2020-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Type/Remove_cv.hpp>
#include "Binding.hpp"
#include "General.hpp"

namespace StdXX::Serialization
{
	class JSONDeserializer
	{
		struct ObjectStackEntry
		{
			String key;
			CommonFileFormats::JsonValue* object;
		};
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
			CommonFileFormats::JsonValue& object = this->CurrentObject();
			binding.value = object[binding.name].StringValue();
			object.Delete(binding.name);
			return *this;
		}

		template <typename T>
		JSONDeserializer& operator>>(const Binding<Optional<T>>& binding)
		{
			CommonFileFormats::JsonValue& parent = this->CurrentObject();
			if(parent.MapValue().Contains(binding.name))
				return *this >> Binding(binding.name, *binding.value);
			return *this;
		}

		template <typename T>
		inline
		Type::EnableIf_t<HasArchiveFunction<JSONDeserializer, T>::value, JSONDeserializer&>
		operator>>(const Binding<T>& binding)
		{
			this->EnterObject(binding.name);
			Archive(*this, binding.value);
			this->LeaveObject();
			return *this;
		}

		template <typename T, typename Type::EnableIf<!Type::IsConst<T>::value, bool>::type = false>
		inline JSONDeserializer& operator>>(T& value)
		{
			this->objectStack.Push({String(), &this->root});
			Archive(*this, value);
			this->objectStack.Pop();
			return *this;
		}

		//Inline
		inline void EnterObject(const String& key)
		{
			CommonFileFormats::JsonValue& parent = this->CurrentObject();
			this->objectStack.Push({key, &parent[key]});
		}

		inline void LeaveObject()
		{
			auto entry = this->objectStack.Pop();

			this->CurrentObject().Delete(entry.key);
		}

		inline bool MorePropertiesExistInCurrentObject() const
		{
			return !this->CurrentObject().MapValue().IsEmpty();
		}

		inline const String& GetNextPropertyName() const
		{
			return this->CurrentObject().MapValue().begin().operator*().key;
		}

	private:
		//Members
		InputStream& inputStream;
		CommonFileFormats::JsonValue root;
		DynamicArray<ObjectStackEntry> objectStack;

		//Properties
		inline CommonFileFormats::JsonValue& CurrentObject()
		{
			return *this->objectStack.Last().object;
		}
		inline const CommonFileFormats::JsonValue& CurrentObject() const
		{
			return *this->objectStack.Last().object;
		}

		//Inline
		inline float64 DeserializeNumber(const String& propertyName)
		{
			CommonFileFormats::JsonValue& object = this->CurrentObject();
			float64 result = object[propertyName].NumberValue();
			object.Delete(propertyName);

			return result;
		}
	};
}