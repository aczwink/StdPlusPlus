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
#include <ACStdLib/Definitions.h>

namespace ACStdLib
{
    //Inline functions
    inline uint64 ComputeGreatestCommonDivisor(uint64 a, uint64 b)
    {
        uint64 tmp;

        while(b)
        {
            tmp = a;
            a = b;
            b = tmp % b;
        }

        return a;
    }

    inline uint64 ComputeLeastCommonMultiple(uint64 a, uint64 b)
    {
        return a * b / ComputeGreatestCommonDivisor(a, b);
    }

    //Class
    class CFraction
    {
    public:
        //Members
        bool isNegative;
        uint64 numerator;
        uint64 denominator;

        //Constructors
        inline CFraction()
        {
            this->isNegative = false;
            this->numerator = 0;
            this->denominator = 1;
        }

        inline CFraction(uint64 numerator, uint64 denominator)
        {
            this->isNegative = false;
            this->numerator = numerator;
            this->denominator = denominator;
        }

        //Inline operators
        inline bool operator==(const CFraction &refRight) const
        {
            return this->isNegative == refRight.isNegative && this->numerator == refRight.numerator && this->denominator == refRight.denominator;
        }

        inline CFraction operator*(uint64 right) const
        {
            return CFraction(this->numerator * right, this->denominator);
        }

        inline CFraction operator*(const CFraction &refRight) const
        {
            return CFraction(this->numerator * refRight.numerator, this->denominator * refRight.denominator);
        }

        inline CFraction &operator*=(const CFraction &refRight)
        {
            this->numerator *= refRight.numerator;
            this->denominator *= refRight.denominator;

            return *this;
        }

        //Inline
        inline float64 Eval() const
        {
            return this->numerator / (float64)this->denominator;
        }

        inline CFraction Reduce() const
        {
            uint64 gcd;
            CFraction result;

            result = *this;
            gcd = ComputeGreatestCommonDivisor(this->numerator, this->denominator);
            if(gcd)
            {
                result.numerator /= gcd;
                result.denominator /= gcd;
            }

            return result;
        }
    };

    //Inline class operators
    inline uint64 operator*(uint16 left, const CFraction &refRight)
    {
        return left * refRight.numerator / refRight.denominator;
    }

    inline uint64 operator*(uint32 left, const CFraction &refRight)
    {
        return left * refRight.numerator / refRight.denominator;
    }

    inline uint64 operator*(uint64 left, const CFraction &refRight)
    {
        return left * refRight.numerator / refRight.denominator;
    }

    inline float64 operator*(float64 left, const CFraction &refRight)
    {
        return left * refRight.numerator / refRight.denominator;
    }

    //Division
    inline uint64 operator/(uint16 left, const CFraction &refRight)
    {
        return left * refRight.denominator / refRight.numerator;
    }

    inline uint64 operator/(uint64 left, const CFraction &refRight)
    {
        return left * refRight.denominator / refRight.numerator;
    }

    inline float64 operator/(float64 left, const CFraction &refRight)
    {
        return left * refRight.denominator / refRight.numerator;
    }
}