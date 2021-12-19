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
#include "XmlSerializer.hpp"
#include "XMLDeserializer.hpp"

namespace StdXX::Serialization
{
	template <typename ArchiveType, typename T>
	inline ArchiveType& operator<<(ArchiveType& serializer, const Binding<Optional<T>>& binding)
	{
		if(binding.value.HasValue())
			serializer << Binding(binding.name, *binding.value);
		return serializer;
	}

	template <typename ArchiveType>
	inline ArchiveType& operator<<(ArchiveType& serializer, const Binding<FileSystem::Path>& binding)
	{
		serializer << Binding(binding.name, binding.value.String());

		return serializer;
	}

	template <typename ArchiveType>
	inline ArchiveType& operator<<(ArchiveType& serializer, const Binding<const FileSystem::Path>& binding)
	{
		serializer << Binding(binding.name, binding.value.String());

		return serializer;
	}

	template <typename ArchiveType>
	inline ArchiveType& operator>>(ArchiveType& deserializer, const Binding<uint8>& binding)
	{
		uint32 tmp;
		deserializer & Binding(binding.name, tmp);
		if(tmp <= Unsigned<uint8>::Max())
			binding.value = tmp;
		else
			NOT_IMPLEMENTED_ERROR;

		return deserializer;
	}

	template <typename ArchiveType>
	inline ArchiveType& operator>>(ArchiveType& deserializer, const Binding<FileSystem::Path>& binding)
	{
		String tmp;
		deserializer & Binding(binding.name, tmp);
		binding.value = tmp;

		return deserializer;
	}

	template <typename ArchiveType>
	inline ArchiveType& operator>>(ArchiveType& deserializer, const Binding<DateTime>& binding)
	{
		String tmp;
		deserializer & Binding(binding.name, tmp);
		binding.value = DateTime::ParseISOString(tmp);

		return deserializer;
	}

    //clang needs this :(
    template <typename T>
    inline XmlSerializer& operator<<(XmlSerializer& serializer, const Binding<Optional<T>>& binding)
    {
        return operator<<<XmlSerializer>(serializer, binding);
    }
    inline XmlSerializer& operator<<(XmlSerializer& serializer, const Binding<FileSystem::Path>& binding)
    {
        return operator<<<XmlSerializer>(serializer, binding);
    }
    inline XmlSerializer& operator<<(XmlSerializer& serializer, const Binding<const FileSystem::Path>& binding)
    {
        return operator<<<XmlSerializer>(serializer, binding);
    }
    inline JSONDeserializer& operator>>(JSONDeserializer& deserializer, const Binding<FileSystem::Path>& binding)
    {
        return operator>><JSONDeserializer>(deserializer, binding);
    }
    inline XMLDeserializer& operator>>(XMLDeserializer& deserializer, const Binding<FileSystem::Path>& binding)
    {
        return operator>><XMLDeserializer>(deserializer, binding);
    }
    inline XMLDeserializer& operator>>(XMLDeserializer& deserializer, const Binding<DateTime>& binding)
    {
        return operator>><XMLDeserializer>(deserializer, binding);
    }
}
