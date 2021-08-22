/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Math/BinaryFloat.hpp>
//Local
#include <Std++/Math/Rounding.hpp>
//Namespaces
using namespace StdXX::Math;

BinaryFloat::BinaryFloat(const DecimalFloat& decimalFloat, uint64 precision)
{
	Rational<Natural> fraction;
	fraction.numerator = decimalFloat.Significand().absValue;

	auto basePowExp = Power(Natural(10), decimalFloat.Exponent().absValue);
	if(decimalFloat.Exponent().isNegative)
	{
		fraction.denominator = Move(basePowExp);

		Rational<Natural> lowerBound = {Power(Natural(2), Natural(precision-1)), 1};
		while(fraction < lowerBound)
		{
			fraction.numerator *= 2; //scale up
			--this->exponent;
		}
	}
	else
	{
		fraction.numerator *= basePowExp;
		fraction.denominator = 1;

		Rational<Natural> upperBound = {Power(Natural(2), Natural(precision)), 1};
		while(fraction > upperBound)
		{
			fraction.denominator *= 2; //scale down
			++this->exponent;
		}
	}

	this->significand = fraction.DivideAndRoundDown();
	auto remainder = fraction.Remainder();
	RoundInteger(this->significand.absValue, remainder, fraction.denominator / 2, RoundingMode::ROUND_TO_NEAREST_TIES_TO_EVEN);
}

//Public methods
float64 BinaryFloat::ClampTo64Bit() const
{
	const uint8 mantissaSize = 52;
	union
	{
		float64 f64;
		struct
		{
			uint64 mantissa: mantissaSize;
			unsigned exponent: 11;
			unsigned sign: 1;
		};
	} value;

	auto minMantissa = Power(2_i64, (int64)mantissaSize);
	auto maxMantissa = Power(2_i64, (int64)(mantissaSize + 1)); //1 bit is implicit in double format
	Natural scaledMantissa = this->significand.absValue;
	int64 exponentDelta = mantissaSize;

	/*TODO: rescale number if precision is not 53
	 * if(scaledMantissa < minMantissa)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: scale up
	}
	else if(scaledMantissa > maxMantissa)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement
		//scale down and round
		while(scaledMantissa > maxMantissa)
		{
			scaledMantissa /= 2; //discard precision
			exponentDelta++;
		}
		//RoundInteger(scaledMantissa64, guardBit, 1_u64, RoundingMode::ROUND_TO_NEAREST_TIES_TO_EVEN);
	}*/
	uint64 mantissa53bit = scaledMantissa.ClampTo64Bit();
	uint64 msbBitMask = (1_u64 << 52_u64);
	uint64 mantissa52bit = mantissa53bit & ~msbBitMask; //msb (52th bit) is implicitly 1 and not stored

	value.sign = 0;
	value.exponent = this->exponent.ClampTo64Bit() + exponentDelta + 1023;
	value.mantissa = mantissa52bit;

	return value.f64;
}

//TODO: for printing to string check https://randomascii.wordpress.com/2012/03/08/float-precisionfrom-zero-to-100-digits-2/