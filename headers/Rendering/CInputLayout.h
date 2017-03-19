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
#include "../Definitions.h"
#include "../Math/Geometry/CVector2.h"
#include "../Math/Geometry/CVector3.h"

namespace ACStdLib
{
    namespace Rendering
    {
        struct SInputAttribute
        {
            uint8 nComponents;
            uint8 offset;
        };

        class ACSTDLIB_API CInputLayout
        {
        private:
            //Members
            uint8 currentAttributeIndex;
            uint8 offset;
            SInputAttribute inputAttributes[3];

        public:
            //Constructor
            inline CInputLayout()
            {
                this->currentAttributeIndex = 0;
                this->offset = 0;
            }

            //Inline
            inline void AddAttribute2()
            {
                this->inputAttributes[this->currentAttributeIndex].nComponents = 2;
                this->inputAttributes[this->currentAttributeIndex].offset = this->offset;

                this->currentAttributeIndex++;
                this->offset += sizeof(Math::CVector2);
            }

            inline void AddAttribute3()
            {
                this->inputAttributes[this->currentAttributeIndex].nComponents = 3;
                this->inputAttributes[this->currentAttributeIndex].offset = this->offset;

                this->currentAttributeIndex++;
                this->offset += sizeof(Math::CVector3);
            }

            inline const SInputAttribute &GetAttribute(uint8 index) const
            {
                return this->inputAttributes[index];
            }

            inline uint8 GetNumberOfAttributes() const
            {
                return this->currentAttributeIndex;
            }

            inline uint8 GetSize() const
            {
                return this->offset;
            }
        };
    }
}