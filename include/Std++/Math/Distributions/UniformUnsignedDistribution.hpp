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
#include <Std++/Math/RandomBitGenerator.hpp>

namespace StdXX::Math
{
	template<typename UnsignedType, typename GeneratorNatType>
	class UniformUnsignedDistribution
	{
		typedef typename Type::CommonType<UnsignedType, GeneratorNatType>::type CalculationType;
	public:
		//Constructor
		inline UniformUnsignedDistribution(RandomBitGenerator<GeneratorNatType>& randomBitGenerator)
				: randomBitGenerator(randomBitGenerator), min(0), max(Unsigned<UnsignedType>::Max())
		{
		}

		inline UniformUnsignedDistribution(RandomBitGenerator<GeneratorNatType>& randomBitGenerator, UnsignedType min, UnsignedType max)
				: randomBitGenerator(randomBitGenerator), min(min), max(max)
		{
		}

		//Inline
		inline UnsignedType Next() const
		{
			CalculationType rngRange = this->randomBitGenerator.Max() - this->randomBitGenerator.Min();
			CalculationType ownRange = this->max - this->min;

			CalculationType ret;
			if(rngRange > ownRange)
			{
				//downscaling
				ownRange++; //avoid 0
				CalculationType scale = rngRange / ownRange;
				CalculationType border = ownRange * scale;
				do
					ret = this->randomBitGenerator.Next() - this->randomBitGenerator.Min();
				while (ret >= border);
				ret /= scale;
			}
			else
			{
				NOT_IMPLEMENTED_ERROR; //upscaling
			}

			return ret + this->min;
		}

	private:
		//Members
		RandomBitGenerator<GeneratorNatType>& randomBitGenerator;
		UnsignedType min;
		UnsignedType max;
	};
}