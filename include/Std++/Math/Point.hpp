/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX
{
	namespace Math
	{
		template <typename ScalarType>
		class Point
		{
		public:
			//Members
			ScalarType x;
			ScalarType y;

			//Constructors
			inline Point()
			{
				this->x = 0;
				this->y = 0;
			}

			inline Point(ScalarType x, ScalarType y)
			{
				this->x = x;
				this->y = y;
			}

			//Inline operators
			inline Point<ScalarType> operator+(const Point<ScalarType> &refRight) const
			{
				return Point(this->x + refRight.x, this->y + refRight.y);
			}

			inline Point<ScalarType> &operator+=(const Point<ScalarType> &rhs)
			{
				this->x += rhs.x;
				this->y += rhs.y;
				return *this;
			}

			inline Point<ScalarType> &operator-=(const Point<ScalarType> &rhs)
			{
				this->x -= rhs.x;
				this->y -= rhs.y;
				return *this;
			}

			inline Point<ScalarType> operator-(const Point<ScalarType> &right) const
			{
				return Point(this->x - right.x, this->y - right.y);
			}

			inline bool operator==(const Point<ScalarType> &rhs) const
			{
				return (this->x == rhs.x) && (this->y == rhs.y);
			}

			inline bool operator!=(const Point<ScalarType> &rhs) const
			{
				return !(*this == rhs);
			}

			//Inline methods
			template <typename CastType>
			inline Point<CastType> Cast() const
			{
				return Point<CastType>(CastType(this->x), CastType(this->y));
			}
		};

		typedef Point<float64> PointD;
	}
}