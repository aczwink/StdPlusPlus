/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX::FileSystem;

TEST_SUITE(POSIXPermissionsTest)
{
    TEST_CASE(ModeShouldNotBeChangedByDecodingAndEncoding)
    {
        const StaticArray<uint32, 4> modes ={{01755, 04563, 02017, 07777}};
        for(uint32 mode : modes)
        {
            POSIXPermissions posixPermissions(0, 0, mode);
            ASSERT_EQUALS(mode, posixPermissions.EncodeMode());
        }
    }
};