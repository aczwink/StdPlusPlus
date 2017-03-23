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
#include <ACStdLibTest.hpp>

unsigned factorial(unsigned n)
{
    if(n == 0)
        return 1;

    return n * factorial(n - 1);
}

TEST_SUITE(FactorialTest)
{
    int someInt = 10;

    TEST(nZeroTest)
    {
        ASSERT(factorial(0) == 1);
    }

    TEST(successTest)
    {
        ASSERT(factorial(10) == 3628800);
    }

    TEST(failureTest)
    {
        ASSERT(someInt != someInt);
    }
};