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
//Class header
#include <ACStdLib/Time/Clock.hpp>
//Global
#include <time.h>
//Namespaces
using namespace ACStdLib;
//Definitions
#define NANO(x) (x * 1000000000)

//Public methods
uint64 Clock::GetElapsedNanoseconds() const
{
	timespec current;

	clock_gettime(CLOCK_MONOTONIC, &current);

	if(current.tv_nsec < this->startNanoSeconds) //end nano seconds are less, that means that at least a second passed
		return NANO(current.tv_sec - this->startSeconds - 1) + ((current.tv_nsec + NANO(1)) - this->startNanoSeconds);

	return NANO(current.tv_sec - this->startSeconds) + (current.tv_nsec - this->startNanoSeconds);
}

void Clock::Start()
{
	timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);

	this->startNanoSeconds = (uint64) t.tv_nsec;
	this->startSeconds = (uint64) t.tv_sec;
}