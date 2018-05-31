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
#include "Std++/Debug.hpp"
#include "../Definitions.h"
#include "../Containers/Strings/StringUtil.h"

namespace StdPlusPlus
{
    namespace Math
    {
        class CComplex
        {
        private:
            //Members
            float64 real;
            float64 imag;

        public:
            //Constructor
            inline CComplex(float64 real = 0, float64 imag = 0)
            {
                this->real = real;
                this->imag = imag;
            }

            //Inline operators
            inline CComplex &operator+=(const CComplex &refRight)
            {
                this->real += refRight.real;
                this->imag += refRight.imag;

                return *this;
            }

            inline CComplex operator*(const CComplex &refRight) const
            {
                return CComplex(this->real * refRight.real - this->imag * refRight.imag, this->real * refRight.imag + refRight.real * this->imag);
            }

            inline CComplex &operator*=(const CComplex &refRight)
            {
                float64 real;

                real = this->real;
                this->real = this->real * refRight.real - this->imag * refRight.imag;
                this->imag = real * refRight.imag + refRight.real * this->imag;

                return *this;
            }

            //Inline
            inline String ToString() const
            {
                if(imag < 0)
                    return String::Number(this->real) + "-i" + String::Number(-this->imag);

                return String::Number(this->real) + "+i" + String::Number(this->imag);
            }
        };
    }
}