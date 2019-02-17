/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Time/DateTime.hpp>
//Global
#include <time.h>
//Namespaces
using namespace StdXX;

//Class functions
DateTime DateTime::Now()
{
	timespec ts;
	int ret = clock_gettime(CLOCK_REALTIME, &ts);
	ASSERT(ret == 0, u8"REPORT THIS PLEASE!");

	return DateTime::FromUnixTimeStampWithMilliSeconds( ts.tv_sec * 1000 + (ts.tv_nsec / 1000000) );
}