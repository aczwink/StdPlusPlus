/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX::Math
{
    //Inline functions
	template<typename NaturalType>
    inline NaturalType ComputeGreatestCommonDivisor(NaturalType a, NaturalType b)
    {
		NaturalType tmp;

        while(b)
        {
            tmp = a;
            a = b;
            b = tmp % b;
        }

        return a;
    }

	template<typename NaturalType>
    inline NaturalType ComputeLeastCommonMultiple(NaturalType a, NaturalType b)
    {
        return a * b / ComputeGreatestCommonDivisor(a, b);
    }

    //Class
    template<typename NaturalType>
    class Rational
    {
    public:
        //Members
        bool isNegative;
		NaturalType numerator;
		NaturalType denominator;

        //Constructors
        inline Rational()
        {
            this->isNegative = false;
            this->numerator = 0;
            this->denominator = 1;
        }

        inline Rational(NaturalType numerator, NaturalType denominator)
        {
            this->isNegative = false;
            this->numerator = numerator;
            this->denominator = denominator;
        }

        //Inline operators
        inline bool operator<(const Rational& rhs) const
		{
        	auto l = this->DivideAndRoundDown();
        	auto r = rhs.DivideAndRoundDown();
        	if(l < r)
        		return true;
        	if(l > r)
        		return false;
        	return this->Remainder() < rhs.Remainder();
		}

		inline bool operator>(const Rational& rhs) const
		{
        	return rhs < *this;
		}

        inline bool operator==(const Rational &refRight) const
        {
            return this->isNegative == refRight.isNegative && this->numerator == refRight.numerator && this->denominator == refRight.denominator;
        }

        inline Rational operator*(NaturalType right) const
        {
            return Rational(this->numerator * right, this->denominator);
        }

        inline Rational operator*(const Rational &refRight) const
        {
            return Rational(this->numerator * refRight.numerator, this->denominator * refRight.denominator);
        }

        inline Rational &operator*=(const Rational &refRight)
        {
            this->numerator *= refRight.numerator;
            this->denominator *= refRight.denominator;

            return *this;
        }

        //Inline
		inline NaturalType DivideAndRoundDown() const
		{
			return this->numerator / this->denominator;
		}

		inline NaturalType Remainder() const
		{
        	return this->numerator % this->denominator;
		}

        inline Rational Reciprocal() const
        {
            return Rational(this->denominator, this->numerator);
        }

        inline Rational Reduce() const
        {
			NaturalType gcd;
            Rational result;

            result = *this;
            gcd = ComputeGreatestCommonDivisor(this->numerator, this->denominator);
            if(gcd != 0)
            {
                result.numerator /= gcd;
                result.denominator /= gcd;
            }

            return result;
        }

		inline float64 ToFloat() const
		{
			return this->numerator / (float64)this->denominator;
		}
    };

	//Binary operators
	template <typename NaturalType>
	inline Rational<NaturalType> operator*(NaturalType lhs, const Rational<NaturalType>& rhs)
	{
		return rhs * lhs;
	}

	template <typename NaturalType>
	inline Rational<NaturalType> operator/(NaturalType lhs, const Rational<NaturalType>& rhs)
	{
		return rhs.Reciprocal() * lhs;
	}
}