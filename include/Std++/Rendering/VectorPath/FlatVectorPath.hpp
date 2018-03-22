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
#include "../../Containers/Array/DynamicArray.hpp"
#include "../../Containers/LinkedList/LinkedList.hpp"
#include "../../Math/Vector2.hpp"

namespace StdPlusPlus
{
    namespace Rendering
    {
        class FlatVectorPath
        {
            friend class VectorPath;
        public:
            struct SPoint
            {
                Math::Vector2s pos;
                Math::Vector2s dir;
                float64 length;
                Math::Vector2s extrusion;
                bool isCorner;
                bool bevelInner;
                bool bevelOuter;
            };

        private:
            //Members
            LinkedList<SPoint> points;
            bool isClosed;
            bool isConvex;

        public:
            //Constructor
            inline FlatVectorPath()
            {
                this->isClosed = false;
                this->isConvex = false;
            }

            //Methods
            void ComputeJoins(float64 w, float64 miterLimit);
            void ComputePointAttributes();

            //Inline
            inline void AddCorner(const Math::Vector2s &refV)
            {
                this->AddPoint(refV, true);
            }

            inline void AddPoint(const Math::Vector2s &refV, bool isCorner)
            {
                this->points.InsertTail({refV, Math::Vector2s(), 0, Math::Vector2s(), isCorner, false, false});
            }

            inline const SPoint &GetPoint(uint32 index) const
            {
                return this->points[index];
            }

            inline const LinkedList<SPoint> &GetPoints() const
            {
                return this->points;
            }

            inline bool IsClosed() const
            {
                return this->isClosed;
            }

            inline bool IsConvex() const
            {
                return this->isConvex;
            }
        };
    }
}