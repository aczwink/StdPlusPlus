/*
* Copyright (c) 2017-2019,2021-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory/MemoryManager.hpp>
//Global
#include <stdlib.h>
//Namespaces
using namespace StdXX::Memory;

class POSIXProcessAllocator : public Allocator
{
public:
	//Methods
	virtual void* Allocate(uint64 size) override
	{
		return malloc(size);
	}

	virtual void Free(void* mem) override
	{
		free(mem);
	}

	virtual void* Reallocate(void* mem, uint64 size) override
	{
		if(!mem)
            return this->Allocate(size);

        return realloc(mem, size);
	}
};

//Public methods
Allocator& MemoryManager::ProcessSystemAllocator()
{
	static POSIXProcessAllocator ppa;
	return ppa;
}
