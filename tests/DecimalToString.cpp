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
#include <StdXXTest.hpp>
using namespace StdXX;

#include <cstdio>
#include <clocale>

static void test(const char8_t* numberString)
{
	float64 expected = strtod(reinterpret_cast<const char *>(numberString), nullptr);
	float64 got = Float<float64>::Parse(numberString);

	if(got != expected)
	{
		printf("DIFFERENCE!\n");
		printf("%a %.64g\n", expected, expected);
		printf("%a %.64g\n", got, got);
		ASSERT(false, u8"Floats are not equal");
	}
}

TEST_SUITE(DecimalToStringTest)
{
	TEST_CASE(ProductionBugs)
	{
		setlocale(LC_NUMERIC, "C");

		test(u8"0.0");
	}

    TEST_CASE(InfiniteLengthInBase2)
	{
		setlocale(LC_NUMERIC, "C");

		/**
		 * Test values are from:
		 * https://www.exploringbinary.com/correct-decimal-to-floating-point-using-big-integers/
		 * https://www.exploringbinary.com/how-glibc-strtod-works/
		 * https://www.exploringbinary.com/glibc-strtod-incorrectly-converts-2-to-the-negative-1075/
		 */

		test(u8"0.1");
		test(u8"3.14159");
		test(u8"1.2345678901234567e22");
		test(u8"1.7976931348623158e308");
		test(u8"2.2250738585072014e-308");

		test(u8"1234.56789012345678901234567890123456789");
		test(u8"123456789012345.678901234567890123456789");
		test(u8"123456789012345.0034375");
		test(u8"123456789012345.0234375");
		test(u8"12345678901234567.8901234567890123456789");
		test(u8"9007199254740991.05");
		test(u8"0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000022250738585072008890245868760858598876504231122409594654935248025624400092282356951787758888037591552642309780950434312085877387158357291821993020294379224223559819827501242041788969571311791082261043971979604000454897391938079198936081525613113376149842043271751033627391549782731594143828136275113838604094249464942286316695429105080201815926642134996606517803095075913058719846423906068637102005108723282784678843631944515866135041223479014792369585208321597621066375401613736583044193603714778355306682834535634005074073040135602968046375918583163124224521599262546494300836851861719422417646455137135420132217031370496583210154654068035397417906022589503023501937519773030945763173210852507299305089761582519159720757232455434770912461317493580281734466552734375");
		test(u8"0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000024703282292062327208828439643411068618252990130716238221279284125033775363510437593264991818081799618989828234772285886546332835517796989819938739800539093906315035659515570226392290858392449105184435931802849936536152500319370457678249219365623669863658480757001585769269903706311928279558551332927834338409351978015531246597263579574622766465272827220056374006485499977096599470454020828166226237857393450736339007967761930577506740176324673600968951340535537458516661134223766678604162159680461914467291840300530057530849048765391711386591646239524912623653881879636239373280423891018672348497668235089863388587925628302755995657524455507255189313690836254779186948667994968324049705821028513185451396213837722826145437693412532098591327667236328125");
	}
};