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
#include "FlatVectorPath.hpp"

namespace StdXX
{
    namespace Rendering
    {
        class VectorPath
        {
            enum class ECommand
            {
                BezierTo,
                Close,
                LineTo,
                MoveTo
            };

        public:
            //Methods
            DynamicArray<FlatVectorPath *> Flatten();

            //Inline
            inline void BezierTo(const Math::Vector2D &refCP, const Math::Vector2D &refTo)
            {
                const auto &refLastPoint = this->points[this->points.GetNumberOfElements() - 1];

                this->commands.Push(ECommand::BezierTo);
                this->points.Push(refLastPoint + 2.0/3.0 * (refCP - refLastPoint));
                this->points.Push(refTo + 2.0/3.0 * (refCP - refTo));
                this->points.Push(refTo);
            }

            inline void BezierTo(const Math::Vector2D &refCP1, const Math::Vector2D &refCP2, const Math::Vector2D &refTo)
            {
                this->commands.Push(ECommand::BezierTo);
                this->points.Push(refCP1);
                this->points.Push(refCP2);
                this->points.Push(refTo);
            }

            inline void Close()
            {
                this->commands.Push(ECommand::Close);
            }

            inline void MoveTo(const Math::Vector2D &refV)
            {
                this->commands.Push(ECommand::MoveTo);
                this->points.Push(refV);
            }

            inline void LineTo(const Math::Vector2D &refV)
            {
                this->commands.Push(ECommand::LineTo);
                this->points.Push(refV);
            }

        private:
            //Members
            DynamicArray<ECommand> commands;
            DynamicArray<Math::Vector2D> points;
        };
    }
}