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
#include <Std++/Errorhandling/Errors/IllegalCodePathError.hpp>

namespace StdXX::Serialization
{
	class XmlSerializer
	{
	public:
		//Constructor
		inline XmlSerializer(OutputStream& outputStream) : writer(outputStream)
		{
			this->inAttributes = false;
		}

		//Operators
		template <typename T>
		inline XmlSerializer& operator&(const Binding<T>& binding)
		{
			return *this << binding;
		}

		template <typename T>
		inline XmlSerializer& operator<<(const Binding<T>& binding)
		{
			if(this->inAttributes)
			{
				this->currentAttribute = binding.name;
				*this << binding.value;
			}
			else
			{
				this->writer.BeginElement(binding.name);
				*this << binding.value;
				this->writer.EndElement();
			}
			return *this;
		}

		inline void operator<<(String& value)
		{
			if(this->inAttributes)
				this->writer.WriteAttribute(this->currentAttribute, value);
			else
				this->writer.WriteText(value);
		}

		inline void operator<<(const String& value)
		{
			*this << const_cast<String&>(value);
		}

		//Inline
		inline void EnterAttributes()
		{
			this->inAttributes = true;
		}

		inline void EnterElement(const String& tagName)
		{
			this->writer.BeginElement(tagName);
		}

		inline void LeaveAttributes()
		{
			this->inAttributes = false;
		}

		inline void LeaveElement()
		{
			this->writer.EndElement();
		}

	private:
		//Members
		CommonFileFormats::XML::Writer writer;
		bool inAttributes;
		String currentAttribute;

		//Operators
		inline void operator<<(bool value)
		{
			*this << (value ? u8"true" : u8"false");
		}

		inline void operator<<(int32 value)
		{
			*this << String::Number(value);
		}

        inline void operator<<(uint32 value)
        {
            *this << String::Number(value);
        }

		inline void operator<<(uint64 value)
		{
			*this << String::Number(value);
		}

		inline void operator<<(const char* value)
		{
			*this << String(value);
		}

		template <typename T>
		inline void operator<<(T& obj)
		{
			ASSERT(!this->inAttributes, u8"Hierarchies are not supported while in attributes");
			Archive(*this, obj);
		}
	};
}