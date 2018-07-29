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
#include <Std++CPUOpt.hpp>
#include "../../Color.hpp"
#include <Std++/Math/Vector2/Vector2.inl>

namespace StdXX
{
    namespace Rendering
    {
        enum class ELineCap
        {
            Butt
        };

        class STDPLUSPLUS_API A2DPathRenderer
        {
        public:
            //Destructor
            virtual ~A2DPathRenderer() {}

            //Abstract
            virtual void BeginPath() = 0;
            virtual void BezierTo(const Math::Vector2D &refCP0, const Math::Vector2D &refCP1, const Math::Vector2D &refTo) = 0;
            virtual void ClosePath() = 0;
            virtual void Fill() = 0;
            virtual void LineTo(const Math::Vector2D &refV) = 0;
            virtual void MoveTo(const Math::Vector2D &refV) = 0;
            virtual void SetStrokeColor(const Color &refStrokeColor) = 0;
            virtual void SetStrokeWidth(float64 strokeWidth) = 0;
            virtual void Stroke() = 0;

            //Inline
            inline void BezierTo(float64 cp0x, float64 cp0y, float64 cp1x, float64 cp1y, float64 toX, float64 toY)
            {
                this->BezierTo(Math::Vector2D(cp0x, cp0y), Math::Vector2D(cp1x, cp1y), Math::Vector2D(toX, toY));
            }

            inline void LineTo(float64 x, float64 y)
            {
                this->LineTo(Math::Vector2D(x, y));
            }

            inline void MoveTo(float64 x, float64 y)
            {
                this->MoveTo(Math::Vector2D(x, y));
            }

            inline void Rectangle(float64 x, float64 y, float64 width, float64 height)
            {
                this->BeginPath();
                this->MoveTo(x, y);
                this->LineTo(x + width, y);
                this->LineTo(x + width, y + height);
                this->LineTo(x, y + height);
                this->ClosePath();
            }

			inline void RoundedRectangle(float64 x, float64 y, float64 width, float64 height, float64 radius)
			{
				constexpr float64 sqrt2 = 1.4142135381698608398437500;
				constexpr float64 kappa = 4 * (sqrt2 - 1) / 3; //for a 90° arc

				this->BeginPath();
				this->MoveTo(x, y + radius);
				this->BezierTo(x, y + kappa * radius, x + kappa * radius, y, x + radius, y); //bottom left curve
				this->LineTo(x + width - radius, y); //bottom line
				this->BezierTo(x + width - kappa * radius, y, x + width, y + kappa * radius, x + width, y + radius); //bottom right curve
				this->LineTo(x + width, y + height - radius); //right line
				this->BezierTo(x + width, y + height - kappa * radius, x + width - kappa * radius, y + height, x + width - radius, y + height); //top right curve
				this->LineTo(x + radius, y + height); //top line
				this->BezierTo(x + kappa * radius, y + height, x, y + height - kappa * radius, x, y + height - radius); //top left curve
				this->ClosePath(); //yields left line
			}
        };
    }
}