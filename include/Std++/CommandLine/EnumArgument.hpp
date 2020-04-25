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
#include "Argument.hpp"

namespace StdXX::CommandLine
{
	template <typename EnumType>
	class EnumArgument : public Argument
	{
	public:
		//Constructor
		inline EnumArgument(const String& name, const String& description) : Argument(name, description)
		{
		}

		//Methods
		String ToString() const override
		{
			String values;
			for(const auto& kv : this->enumMap)
				values += u8"    " + kv.key + u8"\t" + this->descriptionMap[kv.key] + u8"\n";
			if(!values.IsEmpty())
				values = values.SubString(0, values.GetLength()-1);
			return Argument::ToString() + u8" Possible values are:\n" + values;
		}

		//Inline
		inline void AddMapping(const String& string, EnumType enumValue, const String& description)
		{
			this->enumMap.Insert(string, enumValue);
			this->descriptionMap.Insert(string, description);
		}

	private:
		//Members
		Map<String, EnumType> enumMap;
		Map<String, String> descriptionMap;
	};
}