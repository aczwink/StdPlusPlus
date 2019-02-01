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
#include "../Containers/Array/DynamicArray.hpp"
#include "../Streams/InputStream.hpp"
#include "Architecture.hpp"
#include "Instruction.hpp"

namespace StdXX
{
    namespace CodeAnalysis
    {
        class STDPLUSPLUS_API Program
        {
        public:
            //Constructor
            Program(InputStream &refInput, uint32 programSize, Architecture architecture);

            //Destructor
            ~Program();

        private:
            //Members
            DynamicArray<Instruction *> instructions;
        };
    }
}