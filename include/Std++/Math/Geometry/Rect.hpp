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
#include "../../Definitions.h"
#include "../Point.hpp"
#include "../Size.hpp"
#include <Std++/Containers/Strings/String.hpp>

namespace StdPlusPlus
{
    class STDPLUSPLUS_API Rect
    {
    public:
        //Members
        Point origin;
        Size size;

        //Constructors
        inline Rect()
        {
        }

        inline Rect(int32 x, int32 y, int32 width, int32 height)
        {
            this->origin.x = x;
            this->origin.y = y;
            this->size.width = width;
            this->size.height = height;
        }

        inline Rect(const Point &refPoint, const Size &refSize)
        {
            this->origin = refPoint;
            this->size = refSize;
        }

        //Methods
        Rect Intersect(const Rect &refRect) const;

        //Inline methods
        inline bool Contains(int32 x, int32 y) const
        {
            return (x >= this->origin.x && x < this->GetRight()) && (y >= this->origin.y && y < this->GetBottom());
        }

        inline void Enlarge(int32 dx, int32 dy)
        {
            this->origin.x -= dx;
            this->origin.y -= dy;
			this->size.Add(2 * dx, 2 * dy);
        }

        inline int32 GetBottom() const
        {
            return this->origin.y + this->size.height;
        }

        inline int32 GetRight() const
        {
            return this->origin.x + this->size.width;
        }

        inline uint16 &height()
        {
            return this->size.height;
        }

        inline uint16 height() const
        {
            return this->size.height;
        }

        inline bool Intersects(const Rect &refRect) const
        {
            return (this->origin.x <= refRect.GetRight() && refRect.origin.x <= this->GetRight() && this->origin.y <= refRect.GetBottom() && refRect.origin.y <= this->GetBottom());
        }

        inline void Offset(int32 dx, int32 dy)
        {
            this->origin.x += dx;
            this->origin.y += dy;
        }

        inline String ToString() const
		{
			return u8"Rect(" + String::Number(this->origin.x) + u8", " + String::Number(this->origin.y) + u8", "
				   + String::Number(this->size.width) + u8", " + String::Number(this->size.height) + u8")";
		}

        inline uint16 &width()
        {
            return this->size.width;
        }

        inline uint16 width() const
        {
            return this->size.width;
        }

        inline int32 &x()
        {
            return this->origin.x;
        }

        inline int32 x() const
        {
            return this->origin.x;
        }

        inline int32 &y()
        {
            return this->origin.y;
        }

        inline int32 y() const
        {
            return this->origin.y;
        }
    };
}