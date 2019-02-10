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
#pragma once
//Local
#include <Std++/Memory.hpp>

namespace StdXX
{
    namespace Debugging
    {
        class STDPLUSPLUS_API FunctionHook
        {
        public:
            //Constructor
            inline FunctionHook(void* functionAddress) : functionAddress(functionAddress), trampoline(nullptr)
            {
            }

            //Destructor
            inline ~FunctionHook()
            {
                this->UnHook();
				if (this->trampoline)
					VirtualMemoryFree(this->trampoline, this->trampolineSize);
            }

            //Methods
            bool Hook(void* redirectTargetAddress);
            void UnHook();

			//Inline
			inline void Trampoline()
			{
				//TODO: this currently does not work stable! see source code
				void (*f)() = (void(*)())this->trampoline;
				f();
			}

        private:
            //Members
			void* functionAddress;
			uint8 origBytes;
			byte origFuncCode[15];
			void* trampoline;
			uint32 trampolineSize;
        };
    }
}