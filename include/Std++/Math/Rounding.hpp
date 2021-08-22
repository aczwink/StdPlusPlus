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

namespace _stdxx_
{
	template<typename T>
	bool IsNumberEven(const T& number);

	template<>
	bool IsNumberEven<StdXX::Math::Natural>(const StdXX::Math::Natural& number)
	{
		return number.IsEven();
	}

	template<>
	bool IsNumberEven<uint64>(const uint64& number)
	{
		return (number & 1) == 0;
	}
}

namespace StdXX::Math
{
	enum class RoundingMode
	{
		ROUND_TO_NEAREST_TIES_TO_EVEN,
	};

	//Functions
	template<typename ValueType>
	void RoundInteger(ValueType& valueToRound, const ValueType& remainder, const ValueType& half, RoundingMode roundingMode)
	{
		switch(roundingMode)
		{
			case RoundingMode::ROUND_TO_NEAREST_TIES_TO_EVEN:
			{
				if( (remainder > half) or ( (remainder == half) and !_stdxx_::IsNumberEven(valueToRound) ) )
					++valueToRound;
			}
			break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
}