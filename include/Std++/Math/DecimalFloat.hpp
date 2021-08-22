/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include "Integer.hpp"
#include "Rational.hpp"

namespace StdXX::Math
{
	class DecimalFloat
	{
	public:
		//Constructor
		DecimalFloat(const String& string);

		//Properties
		inline const Integer& Exponent() const
		{
			return this->exponent;
		}

		inline const Integer& Significand() const
		{
			return this->significand;
		}

		//Methods
		String ToString() const;

	private:
		//Members
		Integer significand;
		Integer exponent;
	};
}