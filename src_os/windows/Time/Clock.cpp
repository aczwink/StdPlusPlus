/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Time/Clock.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Debug.h>
//Namespaces
using namespace StdPlusPlus;

//Local variables
static LARGE_INTEGER frequency;

//Constructor
Clock::Clock()
{
	QueryPerformanceFrequency(&frequency);
}

//Public methods
uint64 Clock::GetElapsedNanoseconds() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void Clock::Start()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint64 Clock::GetCurrentValue() const
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	return static_cast<uint64>(current.QuadPart * 1000000000 / frequency.QuadPart);
}
