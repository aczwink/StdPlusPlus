/*
* Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/Containers/BinaryTreeMap/BijectiveMap.hpp>

namespace _stdxx_
{
	template<typename T>
	class CodingFormatIdMap : public StdXX::BijectiveMap<T, StdXX::Multimedia::CodingFormatId>
	{
	public:
		//Inline
		inline StdXX::Multimedia::CodingFormatId Get(const T &t) const
		{
			if(this->Contains(t))
				return StdXX::BijectiveMap<T, StdXX::Multimedia::CodingFormatId>::Get(t);
			return StdXX::Multimedia::CodingFormatId::Unknown;
		}
	};
}