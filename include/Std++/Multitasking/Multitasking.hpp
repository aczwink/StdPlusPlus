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
//Global
#ifdef XPC_FEATURE_SSE2
#include <wmmintrin.h>
#endif
//Local
#include <Std++/Time/TimeMisc.hpp>

namespace StdXX
{
    //Functions
    STDPLUSPLUS_API uint32 GetHardwareConcurrency();
    void Thread_Yield();

    //Inline
	inline void Thread_Yield(uint32 repeatCount)
	{
		//inspired by how boost does it
		if(repeatCount < 4)
		{
		}
#ifdef XPC_FEATURE_SSE2
		else if(repeatCount < 16)
		{
			_mm_pause();
		}
#endif
		else if((repeatCount < 32) || (repeatCount & 1))
		{
			Thread_Yield();
		}
		else
		{
			Sleep(1000);
		}
	}
}