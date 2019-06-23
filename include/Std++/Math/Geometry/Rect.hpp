/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "../../Definitions.h"
#include "../Point.hpp"
#include "../Size.hpp"
#include <Std++/Containers/Strings/String.hpp>

namespace StdXX
{
    namespace Math
    {
		template <typename ScalarType>
        class STDPLUSPLUS_API Rect
        {
        public:
            //Members
			Point<ScalarType> origin;
			Size<ScalarType> size;

            //Constructors
            inline Rect()
            {
            }

            inline Rect(ScalarType x, ScalarType y, ScalarType width, ScalarType height)
            {
                this->origin.x = x;
                this->origin.y = y;
                this->size.width = width;
                this->size.height = height;
            }

            inline Rect(const Point<ScalarType> &refPoint, const Size<ScalarType> &refSize)
            {
                this->origin = refPoint;
                this->size = refSize;
            }

            //Methods
            Rect Intersect(const Rect<ScalarType> &refRect) const;

            //Inline methods
			template <typename CastType>
			inline Rect<CastType> Cast() const
			{
				return Rect<CastType>(this->origin.template Cast<CastType>(), this->size.template Cast<CastType>());
			}

            inline bool Contains(ScalarType x, ScalarType y) const
            {
                return (x >= this->origin.x && x < this->GetHorizontalEnd()) && (y >= this->origin.y && y <
																										this->GetVerticalEnd());
            }

            inline void Enlarge(ScalarType dx, ScalarType dy)
            {
                this->origin.x -= dx;
                this->origin.y -= dy;
                this->size.Add(2 * dx, 2 * dy);
            }

			inline Point<ScalarType> GetEnd() const
			{
				return {
					this->GetHorizontalEnd(),
					this->GetVerticalEnd()
				};
			}

            inline ScalarType GetVerticalEnd() const
            {
                return this->origin.y + this->size.height;
            }

            inline ScalarType GetHorizontalEnd() const
            {
                return this->origin.x + this->size.width;
            }

            inline ScalarType &height()
            {
                return this->size.height;
            }

            inline ScalarType height() const
            {
                return this->size.height;
            }

            inline bool Intersects(const Rect<ScalarType> &refRect) const
            {
                return (this->origin.x <= refRect.GetHorizontalEnd() && refRect.origin.x <= this->GetHorizontalEnd() &&
                        this->origin.y <= refRect.GetVerticalEnd() && refRect.origin.y <= this->GetVerticalEnd());
            }

            inline void Offset(ScalarType dx, ScalarType dy)
            {
                this->origin.x += dx;
                this->origin.y += dy;
            }

            inline String ToString() const
            {
                return u8"Rect(" + String::Number(this->origin.x) + u8", " + String::Number(this->origin.y) + u8", "
                       + String::Number(this->size.width) + u8", " + String::Number(this->size.height) + u8")";
            }

            inline ScalarType &width()
            {
                return this->size.width;
            }

            inline ScalarType width() const
            {
                return this->size.width;
            }

            inline ScalarType &x()
            {
                return this->origin.x;
            }

            inline ScalarType x() const
            {
                return this->origin.x;
            }

            inline ScalarType &y()
            {
                return this->origin.y;
            }

            inline ScalarType y() const
            {
                return this->origin.y;
            }
        };

		//Explicit instantiations
		template class StdXX::Math::Rect<uint16>;

		typedef Rect<float64> RectD;
    }
}