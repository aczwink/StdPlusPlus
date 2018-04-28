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
#pragma once
//Local
#include "../Definitions.h"
#include "../Function.hpp"
#include "TimerEventSource.hpp"

namespace StdPlusPlus
{
    class STDPLUSPLUS_API Timer
    {
		friend class TimerEventSource;
    public:
        //Constructor
        Timer(const Function<void()> &timedOutCallback, TimerEventSource &eventSource = *TimerEventSource::globalSource);

        //Destructor
        ~Timer();

        //Methods
		void OneShot(uint64 timeOut_usec);
        void Start(uint64 timeOut_usec);
        void Stop();

	private:
		//Members
		TimerEventSource &eventSource;
		Function<void()> timedOutCallback;
    };
}