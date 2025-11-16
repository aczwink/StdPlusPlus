/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/StaticArray.hpp>

namespace StdXX::Serialization
{
	template <typename T, uint32 N>
	class StringMapping
	{
	public:
		//Constructor
		inline StringMapping(T& value, const StaticArray<Tuple<T, String>, N>& stringMapping) : value(value), stringMapping(stringMapping)
		{
		}

		//Operators
		StringMapping& operator=(const String& string)
		{
			for(const Tuple<T, String>& t : this->stringMapping)
			{
				if(t.template Get<1>() == string)
				{
					this->value = t.template Get<0>();
					return *this;
				}
			}

			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return *this;
		}

		//Methods
		String ToString() const
		{
			for(const Tuple<T, String>& t : this->stringMapping)
			{
				if(t.template Get<0>() == this->value)
					return t.template Get<1>();
			}
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return {};
		}

	private:
		//Members
		T& value;
		const StaticArray<Tuple<T, String>, N>& stringMapping;
	};

	//Functions
	template <typename T, uint32 N>
	inline JSONSerializer& operator<<(JSONSerializer& serializer, const Binding<StringMapping<T, N>>& binding)
	{
		serializer << Binding(binding.name, binding.value.ToString());
		return serializer;
	}

	template <typename ArchiveType, typename T, uint32 N>
	inline ArchiveType& operator>>(ArchiveType& deserializer, const Binding<StringMapping<T, N>>& binding)
	{
		String tmp;
		deserializer & Binding(binding.name, tmp);
		binding.value = tmp;

		return deserializer;
	}

	template <typename T, uint32 N>
	inline void operator<<(XMLSerializer& serializer, StringMapping<T, N>& value)
	{
		serializer << value.ToString();
	}

	//clang needs this :(
    template <typename T, uint32 N>
    constexpr JSONDeserializer& operator>>(JSONDeserializer& deserializer, const Binding<StringMapping<T, N>>& binding)
    {
        return operator>><JSONDeserializer, T, N>(deserializer, binding);
    }
    template <typename T, uint32 N>
    constexpr XMLDeserializer& operator>>(XMLDeserializer& deserializer, const Binding<StringMapping<T, N>>& binding)
    {
        return operator>><XMLDeserializer, T, N>(deserializer, binding);
    }
}