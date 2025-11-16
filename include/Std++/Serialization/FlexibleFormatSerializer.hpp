/*
 * Copyright (c) 2025 Amir Czwink (amir130@hotmail.de)
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
#include "XMLSerializer.hpp"
#include "JSONSerializer.hpp"

namespace StdXX::Serialization
{
	enum class FlexibleFormatType
	{
		JSON,
		XML,
		YAML
	};

	inline String GetFileExtensionOfFormat(FlexibleFormatType formatType)
	{
		switch(formatType)
		{
			case FlexibleFormatType::JSON:
				return u8"json";
			case FlexibleFormatType::XML:
				return u8"xml";
			case FlexibleFormatType::YAML:
				return u8"yaml";
		}
	}

	class FlexibleFormatSerializer
	{
	public:
		//Constructor
		inline FlexibleFormatSerializer(OutputStream& outputStream, FlexibleFormatType type) : outputStream(outputStream)
		{
			switch(type)
			{
				case FlexibleFormatType::JSON:
					this->jsonSerializer = new JSONSerializer(outputStream);
					break;
				case FlexibleFormatType::XML:
					this->xmlSerializer = new XMLSerializer(outputStream);
					break;
				case FlexibleFormatType::YAML:
					this->jsonSerializer = new JSONSerializer(outputStream, true);
					break;
			}
		}

		//Operators
		template <typename T>
		inline FlexibleFormatSerializer& operator<<(const Binding<T>& binding)
		{
			if(!this->jsonSerializer.IsNull())
				*this->jsonSerializer << binding;
			else if(!this->xmlSerializer.IsNull())
				*this->xmlSerializer << binding;
			return *this;
		}

		template <typename T>
		inline FlexibleFormatSerializer& operator<<(const Binding<DynamicArray<T>>& binding)
		{
			if(!this->jsonSerializer.IsNull())
				*this->jsonSerializer << binding;
			else if(!this->xmlSerializer.IsNull())
			{
				this->xmlSerializer->EnterElement(binding.name);
				this->xmlSerializer->EnterAttributes();
				*this->xmlSerializer << Binding(u8"type", u8"list");
				this->xmlSerializer->LeaveAttributes();
				for(const auto& value : binding.value)
					*this->xmlSerializer << Binding(u8"Element", value);
				this->xmlSerializer->LeaveElement();
			}
			return *this;
		}

		//Inline
		inline void EnterObject(const String& objectName)
		{
			if(!this->jsonSerializer.IsNull())
				this->jsonSerializer->EnterObject(objectName);
			else if(!this->xmlSerializer.IsNull())
				this->xmlSerializer->EnterElement(objectName);
		}

		inline void LeaveObject()
		{
			if(!this->jsonSerializer.IsNull())
				this->jsonSerializer->LeaveObject();
			else if(!this->xmlSerializer.IsNull())
				this->xmlSerializer->LeaveElement();
		}

	private:
		//State
		OutputStream& outputStream;
		UniquePointer<JSONSerializer> jsonSerializer;
		UniquePointer<XMLSerializer> xmlSerializer;
	};
}