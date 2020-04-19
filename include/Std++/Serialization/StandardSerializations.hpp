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
#include "XmlSerializer.hpp"
#include "XmlDeserializer.hpp"

namespace StdXX::Serialization
{
	inline void operator<<(XmlSerializer& serializer, const FileSystem::Path& value)
	{
		serializer << value.GetString();
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
}