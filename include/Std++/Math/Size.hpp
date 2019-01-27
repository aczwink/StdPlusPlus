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
#include <Std++/Mathematics.hpp>

namespace StdXX
{
	namespace Math
	{
		template <typename ScalarType>
		class Size
		{
		public:
			//Members
			ScalarType width;
			ScalarType height;

			//Constructors
			inline Size() : width(0), height(0)
			{
			}

			inline Size(ScalarType width, ScalarType height)
			{
				this->width = width;
				this->height = height;
			}

			//Inline operators
			inline Size operator+(const Size &rhs) const
			{
				return Size(this->width + rhs.width, this->height + rhs.height);
			}

			inline Size &operator+=(const Size &rhs)
			{
				this->width += rhs.width;
				this->height += rhs.height;

				return *this;
			}

			inline Size operator*(ScalarType i) const
			{
				return Size(this->width * i, this->height * i);
			}

			inline Size operator/(ScalarType i) const
			{
				return Size(this->width / i, this->height / i);
			}

			inline bool operator==(const Size<ScalarType> &rhs) const
			{
				return (this->width == rhs.width) && (this->height == rhs.height);
			}

			inline bool operator!=(const Size<ScalarType> &rhs) const
			{
				return !(*this == rhs);
			}

			//Inline
			inline void Add(ScalarType dx, ScalarType dy)
			{
				this->width = Math::Max(ScalarType(this->width + dx), ScalarType(0));
				this->height = Math::Max(ScalarType(this->height + dy), ScalarType(0));
			}

			template <typename CastType>
			inline Size<CastType> Cast() const
			{
				return Size<CastType>(CastType(this->width), CastType(this->height));
			}

			inline Size Max(const Size &rhs) const
			{
				return Size(Math::Max(this->width, rhs.width), Math::Max(this->height, rhs.height));
			}
		};

		typedef Size<float32> SizeS;
		typedef Size<float64> SizeD;
	}
}