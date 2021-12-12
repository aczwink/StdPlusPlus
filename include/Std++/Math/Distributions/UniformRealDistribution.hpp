/*
 * Copyright (c) 2019,2021 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	namespace Math
	{
		template<typename RealType, typename GeneratorNatType>
		class UniformRealDistribution
		{
		public:
			//Constructor
			inline UniformRealDistribution(RandomBitGenerator<GeneratorNatType>& randomBitGenerator, RealType min, RealType max)
				: randomBitGenerator(randomBitGenerator)
			{
				this->min = min;
				this->max = max;
			}

			//Inline
			inline RealType Next() const
			{
				return this->randomBitGenerator.template GenerateCanonicalFloat<RealType, sizeof(RealType)*8>() * (this->max - this->min) + this->min;
			}

		private:
			//Members
			RandomBitGenerator<GeneratorNatType>& randomBitGenerator;
			RealType min;
			RealType max;
		};
	}
}