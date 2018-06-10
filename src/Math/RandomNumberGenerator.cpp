/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/RandomNumberGenerator.hpp>
//Global
#include <cstdlib>
#include <ctime>
//Namespaces
using namespace StdPlusPlus::Math;

//Constructor
RandomNumberGenerator::RandomNumberGenerator()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

RandomNumberGenerator::RandomNumberGenerator(uint32 seed)
{
	srand(seed);
}

//Public methods
uint32 RandomNumberGenerator::Max() const
{
	return RAND_MAX;
}

uint32 RandomNumberGenerator::Next()
{
	return static_cast<uint32>(rand());
}
