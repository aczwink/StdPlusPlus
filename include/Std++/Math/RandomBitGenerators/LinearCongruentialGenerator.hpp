/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Signed.hpp>
#include <Std++/Type.hpp>
#include <Std++/Unsigned.hpp>

namespace StdXX
{
	namespace Math
	{
		template <typename NatType, NatType a, NatType c, NatType m>
		class LinearCongruentialGenerator : public RandomBitGenerator<NatType>
		{
			/**
			 * We have to show that a * state + c <= Unsigned<T>::Max().
			 * Maximum state is m - 1 (also for m == 0, because of unsigned wraparound).
			 * I.e. we have to show that a * (m-1) + c <= Unsigned<T>::Max()
			 * I.e. a * (m-1) <= Unsigned<T>::Max() - c
			 * I.e. m-1 <= (Unsigned<T>::Max() - c) / a
			 */
			template <typename ComputeNatType, ComputeNatType _a, ComputeNatType _c, ComputeNatType _m,
			        bool inRange = m-1 <= (Unsigned<ComputeNatType>::Max() - c) / a,
			        bool canUseSchrage = _m % _a < _m / _a>
			struct LCG_Mod
			{
				static inline ComputeNatType Compute(ComputeNatType state);
			};

			template <typename ComputeNatType, ComputeNatType _a, ComputeNatType _c, ComputeNatType _m>
			struct LCG_Mod<ComputeNatType, _a, _c, _m, false, true>
			{
				//Schrage
				static inline ComputeNatType Compute(ComputeNatType state)
				{
					if(_a == 1)
					{
						state %= _m;
					}
					else
					{
						constexpr ComputeNatType q = _m / _a;
						constexpr ComputeNatType r = _m % _a;

						ComputeNatType t1 = _a * (state % q);
						ComputeNatType t2 = r * (state / q);
						if (t1 >= t2)
							state = t1 - t2;
						else
							state = _m - t2 + t1;
					}

					if (_c != 0)
					{
						const ComputeNatType d = _m - state;
						if (d > _c)
							state += _c;
						else
							state = _c - d;
					}
					return state;
				}
			};

		public:
			//Constructor
			inline LinearCongruentialGenerator(NatType seed = 1)
			{
				this->Seed(seed);
			}

			//Methods
			NatType Max() const override
			{
				return m - 1;
			}

			NatType Min() const override
			{
				return (c == 0) ? 1 : 0;
			}

			NatType Next() override
			{
				this->state = LCG_Mod<NatType, a, c, m>::Compute(this->state);
				return this->state;
			}

			void Seed(NatType seed) override
			{
				if((c == 0) && (seed == 0))
					this->state = 1;
				else if(m != 0)
					this->state = seed % m;
				else
					this->state = seed;
			}

		private:
			//Members
			NatType state;
		};

		typedef LinearCongruentialGenerator<uint32, 48271, 0, Signed<int32>::Max()> MinStdRand;
	}
}