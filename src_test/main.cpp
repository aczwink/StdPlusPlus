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
#include <StdXXTest.hpp>
//Namespaces
using namespace StdXX;

//Functions
int32 Main(const String &programName, const FixedArray<String> &args)
{
	//evaluate arguments
	if(args.GetNumberOfElements() > 1)
	{
		NOT_IMPLEMENTED_ERROR; //TODO: thinking about output type xml or human readable or so
	}
	else if(args.GetNumberOfElements() == 1)
	{
		DynamicArray<String> parts = args[0].Split(u8"::");

		if(parts.GetNumberOfElements() == 2)
		{
			if(!StdPlusPlusTest::TestManager::GetInstance().RunTestCase(parts[0], parts[1]))
			{
				stdErr << u8"ERROR: Could not find test suite or test case." << endl;
				return EXIT_FAILURE;
			}
		}
		else if(parts.GetNumberOfElements() == 1)
		{
			if(!StdPlusPlusTest::TestManager::GetInstance().RunTestSuite(parts[0]))
			{
				stdErr << u8"ERROR: Could not find test suite." << endl;
				return EXIT_FAILURE;
			}
		}
		else
		{
			stdErr << u8"ERROR: Wrong test suite/case string." << endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		StdPlusPlusTest::TestManager::GetInstance().RunAllTests();
	}

	return EXIT_SUCCESS;
}