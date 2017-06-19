/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include <ACStdLibCPUOpt.hpp>
#include "ACStdLib/Color.hpp"

namespace ACStdLib
{
    namespace Rendering
    {
        enum class ELineCap
        {
            Butt
        };

        class ACSTDLIB_API A2DPathRenderer
        {
        public:
            //Destructor
            virtual ~A2DPathRenderer() {}

            //Abstract
            virtual void BeginPath() = NULL;
            virtual void BezierTo(const vec2f64 &refTo, const vec2f64 &refCP0, const vec2f64 &refCP1) = NULL;
            virtual void ClosePath() = NULL;
            virtual void Fill() = NULL;
            virtual void LineTo(const vec2f64 &refV) = NULL;
            virtual void MoveTo(const vec2f64 &refV) = NULL;
            virtual void SetStrokeColor(const Color &refStrokeColor) = NULL;
            virtual void SetStrokeWidth(float64 strokeWidth) = NULL;
            virtual void Stroke() = NULL;

            //Inline
            inline void BezierTo(float64 cp0x, float64 cp0y, float64 cp1x, float64 cp1y, float64 toX, float64 toY)
            {
                this->BezierTo(vec2f64(cp0x, cp0y), vec2f64(cp1x, cp1y), vec2f64(toX, toY));
            }

            inline void LineTo(float64 x, float64 y)
            {
                this->LineTo(vec2f64(x, y));
            }

            inline void MoveTo(float64 x, float64 y)
            {
                this->MoveTo(vec2f64(x, y));
            }

            inline void Rectangle(float64 x, float64 y, float64 width, float64 height)
            {
                this->MoveTo(x, y);
                this->LineTo(x + width, y);
                this->LineTo(x + width, y + height);
                this->LineTo(x, y + height);
                this->ClosePath();
            }
        };
    }
}