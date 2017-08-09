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
#pragma once
//Local
#include "../Definitions.h"

namespace ACStdLib
{
	/**
	 * A monotonic clock that can be used to measure time difference.
	 * It never decreases and does not get affected by system time changes (NTP oder change by user etc.).
	 *
	 * The reference time point for the clock is unspecified and therefore unknown.
	 */
	class Clock
	{
	public:
		//Methods
		uint64 GetElapsedNanoseconds() const;
		/**
		 * Sets the start point for time differences.
		 */
		void Start();

		//Inline
		inline uint64 GetElapsedMicroseconds() const
		{
			return this->GetElapsedNanoseconds() / 1000;
		}

	private:
		//Members
		/**
		 * The start point for the clock in seconds since the unknown reference point.
		 */
		uint64 startSeconds;
		/**
		 * The additional nanoseconds for the start point.
		 */
		uint64 startNanoSeconds;
	};
}