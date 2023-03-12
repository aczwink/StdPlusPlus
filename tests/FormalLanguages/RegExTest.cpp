/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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

TEST_SUITE(RegExTest)
{
    TEST_CASE(emptyTests)
    {
        RegEx regEx(u8"");

        String string1 = u8"";
        ASSERT(regEx.Matches(string1), u8"RegEx '' should match string ''");

        String string2 = u8"abc";
        ASSERT(regEx.Matches(string2), u8"RegEx '' should match string 'abc'");
    }

    TEST_CASE(simpleTests)
    {
        String string = u8"ab";

        RegEx regEx1(u8"a");
        ASSERT(regEx1.Matches(string), u8"RegEx 'a' should match string 'ab'");

        RegEx regEx2(u8"b");
        ASSERT(!regEx2.Matches(string), u8"RegEx 'b' should not match string 'ab'");

        RegEx regEx3(u8"ab");
        ASSERT(regEx3.Matches(string), u8"RegEx 'ab' should match string 'ab'");

        RegEx regEx4(u8"(ab)");
        ASSERT(regEx4.Matches(string), u8"RegEx '(ab)' should match string 'ab'");
    }

    TEST_CASE(starTests)
    {
        RegEx regEx(u8"ab*c");

        String string1 = u8"ac";
        ASSERT(regEx.Matches(string1), u8"RegEx 'ab*c' should match string 'ac'");

        String string2 = u8"abc";
        ASSERT(regEx.Matches(string2), u8"RegEx 'ab*c' should match string 'abc'");

        String string3 = u8"abbbc";
        ASSERT(regEx.Matches(string3), u8"RegEx 'ab*c' should match string 'abbbc'");
    }

    TEST_CASE(plusTests)
    {
        RegEx regEx(u8"ab+c");

        String string1 = u8"ac";
        ASSERT(!regEx.Matches(string1), u8"RegEx 'ab+c' should not match string 'ac'");

        String string2 = u8"abc";
        ASSERT(regEx.Matches(string2), u8"RegEx 'ab+c' should match string 'abc'");

        String string3 = u8"abbbc";
        ASSERT(regEx.Matches(string3), u8"RegEx 'ab+c' should match string 'abbbc'");
    }

    TEST_CASE(zeroOrOneTests)
    {
        RegEx regEx(u8"ab?c");

        String string1 = u8"ac";
        ASSERT(regEx.Matches(string1), u8"RegEx 'ab?c' should match string 'ac'");

        String string2 = u8"abc";
        ASSERT(regEx.Matches(string2), u8"RegEx 'ab?c' should match string 'abc'");
    }

    TEST_CASE(dotTests)
    {
        RegEx regEx(u8"a.c");

        String string1 = u8"ac";
        ASSERT(!regEx.Matches(string1), u8"RegEx 'a.c' should not match string 'ac'");

        String string2 = u8"abc";
        ASSERT(regEx.Matches(string2), u8"RegEx 'a.c' should match string 'abc'");
    }

    TEST_CASE(characterClassesTests)
    {
        RegEx regEx1(u8"[ab]");
        RegEx regEx2(u8"[^a-c]");

        String string1 = u8"ac";
        ASSERT(regEx1.Matches(string1), u8"RegEx '[ab]' should match string 'ac'");
        ASSERT(!regEx2.Matches(string1), u8"RegEx '[^a-c]' should not match string 'ac'");

        String string2 = u8"d";
        ASSERT(!regEx1.Matches(string2), u8"RegEx '[ab]' should not match string 'd'");
        ASSERT(regEx2.Matches(string2), u8"RegEx '[^a-c]' should match string 'd'");
    }

    TEST_CASE(orTests)
    {
        RegEx regEx(u8"a|b");

        String string1 = u8"a";
        ASSERT(regEx.Matches(string1), u8"RegEx 'a|b' should match string 'a'");

        String string2 = u8"b";
        ASSERT(regEx.Matches(string2), u8"RegEx 'a|b' should match string 'b'");
    }
}