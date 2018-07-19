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
#include "../Definitions.h"

namespace StdPlusPlus
{
	namespace Math
	{
		class STDPLUSPLUS_API RandomNumberGenerator
		{
		public:
			//Constructors
			RandomNumberGenerator();
			RandomNumberGenerator(uint32 seed);

			//Methods
			uint32 Max() const;
			/**
			 * Returns the next random number in range [Min(), Max()]
			 * @return
			 */
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

			inline float64 NextFloat()
			{
				return (this->Next() - this->Min()) / float64(this->Max());
			}
		};
	}
}