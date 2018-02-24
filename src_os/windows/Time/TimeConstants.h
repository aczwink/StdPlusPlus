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
//Global
#include <Windows.h>
//Local
#include <Std++/Definitions.h>

inline int64 FileTimeToMilliseconds(const FILETIME &refFT)
{
	int64 timeStamp;

	const uint64 FILETIME_TO_EPOCH_BIAS = 116444736000000000i64; //Number of 100 nanosecond units from 1/1/1601 to 1/1/1970

	timeStamp = ((uint64)refFT.dwHighDateTime << 32) | ((uint64)refFT.dwLowDateTime);
	timeStamp -= FILETIME_TO_EPOCH_BIAS;
	timeStamp /= (10 * 1000); //scale to ms

	return timeStamp;
}