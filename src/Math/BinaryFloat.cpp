/*
 * Copyright (c) 2021-2024 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;
using namespace StdXX::Math;

//Very good explanation of the here implemented algorithm: https://www.exploringbinary.com/correct-decimal-to-floating-point-using-big-integers/

//Local functions
/**
 * If number r is in range 2^(x-1) <= r < 2^x this method returns x
 */
static int64 ComputeFractionUpperBoundPrecisionBase2(const Rational<Natural> &fraction)
{
	uint64 precision = Math::Log({2}, fraction.DivideAndRoundDown()).ClampTo64Bit();
	if(precision == 0)
	{
		Rational<Natural> tmp = {1, 2}; //we start with 1/2 and not 1/1 or otherwise this would return the lower and not upper bound
		while(tmp > fraction)
		{
			tmp.denominator *= 2;
			precision--;
		}
	}
	return precision;
}

static void RescaleToPrecision(Rational<Natural>& fraction, Integer& exponent, uint64 desiredPrecision)
{
	int64 currentPrecision = ComputeFractionUpperBoundPrecisionBase2(fraction);
	if((int64)desiredPrecision > currentPrecision)
	{
		//scale up
		uint64 delta = desiredPrecision - currentPrecision;
		fraction.numerator *= Math::Power(Natural(2), Natural(delta));
		exponent -= delta;
	}
	else if(desiredPrecision == currentPrecision)
	{
	}
	else
	{
		//scale down
		uint64 delta = currentPrecision - desiredPrecision;
		fraction.denominator *= Math::Power(Natural(2), Natural(delta));
		exponent += delta;
	}


	//scale to between 2^(precision-1) and 2^precision
	Rational<Natural> lowerBound = {Power(Natural(2), Natural(desiredPrecision-1)), 1};
	while(fraction < lowerBound)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: shouldn't happen anymore. test :)
		/*fraction.numerator *= 2; //scale up
		--this->exponent;*/
	}

	Rational<Natural> upperBound = {Power(Natural(2), Natural(desiredPrecision)), 1};
	while(fraction > upperBound)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: shouldn't happen anymore. test :)
		/*fraction.denominator *= 2; //scale down
		++this->exponent;*/
	}
}

//Constructor
BinaryFloat::BinaryFloat(const DecimalFloat& decimalFloat, uint64 precision)
{
	if(decimalFloat.Significand().absValue == Natural())
		return; //avoid endless loops for 0

	//represent decimal float as as fraction
	Rational<Natural> fraction;
	fraction.numerator = decimalFloat.Significand().absValue;

	auto basePowExp = Power(Natural(10), decimalFloat.Exponent().absValue);
	if(decimalFloat.Exponent().isNegative)
	{
		fraction.denominator = Move(basePowExp);
	}
	else
	{
		fraction.numerator *= basePowExp;
		fraction.denominator = 1;
	}

	RescaleToPrecision(fraction, this->exponent, precision);

	//eval and round
	auto eval = fraction.numerator.DivMod(fraction.denominator);
	this->significand = Move(eval.Get<0>());
	this->significand.isNegative = decimalFloat.Significand().isNegative;
	auto remainder = Move(eval.Get<1>());
	RoundInteger(this->significand.absValue, remainder, fraction.denominator / 2, RoundingMode::ROUND_TO_NEAREST_TIES_TO_EVEN);
}

//Public methods
float64 BinaryFloat::ClampTo64Bit() const
{
	const uint8 mantissaSize = 52;
	const uint16 exponentBias = 1023;
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

	uint64 mantissa53bit = this->significand.absValue.ClampTo64Bit();



	//rescale if exponent is out of bounds
	const int64 rawExponent = this->exponent.ClampTo64Bit();
	int64 realExponent = rawExponent + mantissaSize;

	if(realExponent < -exponentBias)
	{
		int64 amount = -realExponent - exponentBias;
		mantissa53bit >>= amount;
		realExponent += amount;
	}

	//store
	value.sign = this->significand.isNegative ? 1 : 0;
	value.exponent = (uint16)(realExponent + exponentBias);

	if(mantissa53bit == 0)
	{
		//zero is denormalized in IEEE 754
		value.exponent = 0;
		value.mantissa = 0;
	}
	else if(value.exponent == 0)
	{
		//denormalized number, the mantissa is preceded by a zero, loosing one bit of precision
		value.mantissa = mantissa53bit >> 1;
	}
	else
	{
		//normalized number, msb (52th bit) is implicitly 1 and not stored
		uint64 msbBitMask = (1_u64 << 52_u64);
		uint64 mantissa52bit = mantissa53bit & ~msbBitMask;

		value.mantissa = mantissa52bit;
	}

	return value.f64;
}

//TODO: for printing to string check https://randomascii.wordpress.com/2012/03/08/float-precisionfrom-zero-to-100-digits-2/