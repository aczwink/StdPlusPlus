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
#include <Std++/Tuple.hpp>
#include "../_Backends/DirectoryIteratorState.hpp"
#include "../SmartPointers/AutoPointer.hpp"
#include "FileSystemNode.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API DirectoryIterator
    {
    public:
        //Constructors
        inline DirectoryIterator(_stdxx_::DirectoryIteratorState *iteratorState) : iteratorState(iteratorState)
		{
		}

		//move ctor
		inline DirectoryIterator(DirectoryIterator &&other) : iteratorState(other.iteratorState)
		{
            other.iteratorState = nullptr;
        }

        //Destructor
        inline ~DirectoryIterator()
		{
			if(this->iteratorState)
				delete this->iteratorState;
		}

        //Operators
        inline DirectoryIterator &operator++() //Prefix++
		{
			this->iteratorState->Next();
			return *this;
		}

		inline bool operator==(const DirectoryIterator &other) const
		{
			if(this->iteratorState != nullptr)
				return this->iteratorState->Equals(other.iteratorState);
			if(other.iteratorState != nullptr)
				return other.iteratorState->Equals(this->iteratorState);
			return true; //both are nullptr
		}

        inline bool operator!=(const DirectoryIterator &other) const
		{
			return !(*this == other);
		}

        //Inline operators
        inline String operator*()
        {
			return this->iteratorState->GetCurrent();
        }

    private:
        //Members
		_stdxx_::DirectoryIteratorState *iteratorState;
    };
}