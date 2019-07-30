/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Float.hpp>
#include "../Definitions.h"

namespace StdXX
{
	namespace Math
	{
		/**
		 * Ideally uniform, i.e. every value in the range has equal probability to occur.
		 */
		template <typename NatType>
		class RandomBitGenerator
		{
		public:
			//Abstract
			virtual NatType Max() const = 0;
			virtual NatType Min() const = 0;
			virtual NatType Next() = 0;
			virtual void Seed(NatType seed) = 0;

			//Methods
			template <typename FloatType, uint8 _nBits>
			FloatType GenerateCanonicalFloat()
			{
				uint8 nBitsToGen = Math::Min(_nBits, Float<FloatType>::NumberOfDigitsInMantissa());

				const NatType min = this->Min();

				const FloatType r = (this->Max() - min) + 1;
				const FloatType log2r = log(r) / log(FloatType(2));
				const uint32 k = uint32(Math::Max(FloatType(1), ceil(log2r)));

				FloatType sum = 0;
				FloatType factor = 1;
				for(uint32 i = 0; i < k; i++)
				{
					sum += (this->Next() - min) * factor;
					factor *= r;
				}

				return sum / factor;
			}
		};
	}
}

/*
namespace StdXX
{
	namespace Math
	{
		class STDPLUSPLUS_API RandomBitGenerator
		{
		public:
			//Constructors
			RandomBitGenerator();
			RandomBitGenerator(uint32 seed);

			//Methods
			uint32 Max() const;
			/**
			 * Returns the next random number in range [Min(), Max()]
			 * @return
			 *//*
			uint32 Next();

			//Inline
			inline uint32 Min() const
			{
				return 0;
			}

			inline uint32 Next(uint32 min, uint32 max)
			{
				return min + (this->Next() % max);
			}

			/**
			 * Returns the next random floating point number in range[0, 1]
			 * @return
			 *//*
			inline float64 NextFloat()
			{
				return (this->Next() - this->Min()) / float64(this->Max());
			}
		};
	}
}*/