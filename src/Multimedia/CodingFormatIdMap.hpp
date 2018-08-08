/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>

namespace _stdxx_
{
	template<typename T>
	class CodingFormatIdMap
	{
	public:
		//Inline
		inline StdXX::Multimedia::CodingFormatId GetId(const T &t) const
		{
			if (this->map.Contains(t))
				return this->map[t];
			return CodingFormatId::Unknown;
		}

		inline void Insert(const T &t, StdXX::Multimedia::CodingFormatId id)
		{
			this->map.Insert(t, id);
			this->back.Insert(id, t);
		}

		//For range-based loop
		typename StdXX::Map<T, StdXX::Multimedia::CodingFormatId>::Iterator begin()
		{
			return this->map.begin();
		}

		typename StdXX::Map<T, StdXX::Multimedia::CodingFormatId>::ConstIterator begin() const
		{
			return this->map.begin();
		}

		typename StdXX::Map<T, StdXX::Multimedia::CodingFormatId>::Iterator end()
		{
			return this->map.end();
		}

		typename StdXX::Map<T, StdXX::Multimedia::CodingFormatId>::ConstIterator end() const
		{
			return this->map.end();
		}

	private:
		//Members
		StdXX::Map<T, StdXX::Multimedia::CodingFormatId> map;
		StdXX::Map<StdXX::Multimedia::CodingFormatId, T> back;
	};
}