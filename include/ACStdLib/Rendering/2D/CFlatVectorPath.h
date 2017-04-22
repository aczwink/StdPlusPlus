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
#include "../../Containers/CArray/CArray.h"
#include "../../Containers/CLinkedList/CLinkedList.h"
#include "../../Math/Geometry/CVector2.h"

namespace ACStdLib
{
    namespace Rendering
    {
        class CFlatVectorPath
        {
            friend class CVectorPath;
        public:
            struct SPoint
            {
                Math::CVector2 pos;
                Math::CVector2 dir;
                float64 length;
                Math::CVector2 extrusion;
                bool isCorner;
                bool bevelInner;
                bool bevelOuter;
            };

        private:
            //Members
            CLinkedList<SPoint> points;
            bool isClosed;
            bool isConvex;

        public:
            //Constructor
            inline CFlatVectorPath()
            {
                this->isClosed = false;
                this->isConvex = false;
            }

            //Methods
            void ComputeJoins(float64 w, float64 miterLimit);
            void ComputePointAttributes();

            //Inline
            inline void AddCorner(const Math::CVector2 &refV)
            {
                this->AddPoint(refV, true);
            }

            inline void AddPoint(const Math::CVector2 &refV, bool isCorner)
            {
                this->points.InsertTail({refV, Math::CVector2(), 0, Math::CVector2(), isCorner, false, false});
            }

            inline const SPoint &GetPoint(uint32 index) const
            {
                return this->points[index];
            }

            inline const CLinkedList<SPoint> &GetPoints() const
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