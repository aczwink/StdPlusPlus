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
//corresponding header
#include <Std++/Memory.hpp>
//Global
#include <stdlib.h>
//Local
#include <Std++/Debug.hpp>

void *StdXX::MemoryAllocate(uint32 size)
{
    return malloc(size);
}

void StdXX::MemoryFree(void *pMem)
{
    free(pMem);
}

void *StdXX::MemoryReallocate(void *pMem, uint32 size)
{
    if(!pMem)
        return MemoryAllocate(size);

    return realloc(pMem, size);
}

void* StdXX::VirtualMemoryAllocate(uint32 size, MemoryProtection protection)
{
	NOT_IMPLEMENTED_ERROR; //TODO: probably implement using mmap
}

void StdXX::VirtualMemoryProtect(void *pMemoryRegion, uint32 size, MemoryProtection protection)
{
	NOT_IMPLEMENTED_ERROR; //TODO: probably implement using mprotect
}

void StdXX::VirtualMemoryFree(void* addr, uint32 size)
{
	NOT_IMPLEMENTED_ERROR; //TODO: probably implement using munmap
}