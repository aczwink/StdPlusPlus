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
#include <Std++/Math/Natural.hpp>
#include <Std++/Math/Rational.hpp>

namespace StdXX::Multimedia
{
	class TimeScale : public Math::Rational<uint64>
	{
	public:
		//Constructors
		inline TimeScale()
		{
		}

		inline TimeScale(uint64 num, uint64 den) : Rational<uint64>(num, den)
		{
		}

		//Inline
		inline uint64 Rescale(uint64 ts) const
		{
			return (ts / *this).RoundDown();
		}

		inline uint64 Rescale(uint64 ts, const TimeScale& targetTimeScale) const
		{
			if(ts == Unsigned<uint64>::Max())
				return ts;

			Rational<Math::Natural> target(targetTimeScale.numerator, targetTimeScale.denominator);
			Rational<Math::Natural> source(this->numerator, this->denominator);

			return (Math::Natural(ts) * source * target.Reciprocal()).RoundDown().RoundDown();
		}
	};
}