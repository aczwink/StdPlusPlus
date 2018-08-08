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
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX;

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
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	uint64 s = static_cast<uint64>(current.QuadPart / frequency.QuadPart) - this->startSeconds;
	uint64 ns = static_cast<uint64>((current.QuadPart % frequency.QuadPart) * 1000000000 / frequency.QuadPart);
	
	if (ns < this->startNanoSeconds)
	{
		s--;
		ns += 1000000000;
	}
	ns -= this->startNanoSeconds;
	
	return s * 1000000000 + ns;
}

void Clock::Start()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	this->startSeconds = static_cast<uint64>(current.QuadPart / frequency.QuadPart);
	this->startNanoSeconds = static_cast<uint64>( (current.QuadPart % frequency.QuadPart) * 1000000000 / frequency.QuadPart);
}

uint64 Clock::GetCurrentValue() const
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	return static_cast<uint64>(current.QuadPart * 1000000000 / frequency.QuadPart);
}
