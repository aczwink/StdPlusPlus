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
#include "../Definitions.h"

namespace StdXX
{
    namespace Math
    {
        //Forward declarations
        template<typename ScalarType>
        class Radian;

        template<typename ScalarType>
        class STDPLUSPLUS_API Degree
        {
        public:
            //Members
            ScalarType value;

            //Constructors
            inline Degree()
            {
                this->value = 0;
            }

            inline Degree(ScalarType value)
            {
                this->value = value;
            }

            Degree(const Radian<ScalarType> &refRadian);

            //Operators
            inline Degree &operator+=(const Degree &refRight)
            {
                this->value += refRight.value;

                return *this;
            }

            inline Degree operator-() const
            {
                return Degree(-this->value);
            }

            inline Degree operator-(const Degree &refRight) const
            {
                return Degree(this->value - refRight.value);
            }

            inline bool operator<(const Degree &refRight)
            {
                return this->value < refRight.value;
            }

            inline bool operator>(const Degree &refRight)
            {
                return this->value > refRight.value;
            }
        };
    }
}