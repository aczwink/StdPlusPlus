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
#include "FlatVectorPath.hpp"

namespace ACStdLib
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
            inline void BezierTo(const Math::Vector2 &refCP, const Math::Vector2 &refTo)
            {
                const auto &refLastPoint = this->points[this->points.GetNumberOfElements() - 1];

                this->commands.Push(ECommand::BezierTo);
                this->points.Push(refLastPoint + 2.0f/3.0f * (refCP - refLastPoint));
                this->points.Push(refTo + 2.0f / 3.0f * (refCP - refTo));
                this->points.Push(refTo);
            }

            inline void BezierTo(const Math::Vector2 &refCP1, const Math::Vector2 &refCP2, const Math::Vector2 &refTo)
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

            inline void MoveTo(const Math::Vector2 &refV)
            {
                this->commands.Push(ECommand::MoveTo);
                this->points.Push(refV);
            }

            inline void LineTo(const Math::Vector2 &refV)
            {
                this->commands.Push(ECommand::LineTo);
                this->points.Push(refV);
            }

        private:
            //Members
            DynamicArray<ECommand> commands;
            DynamicArray<Math::Vector2> points;
        };
    }
}