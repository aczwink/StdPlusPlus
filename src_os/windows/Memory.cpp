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
//corresponding header
#include <Std++/Memory.hpp>
//Global
#include <Windows.h>
//Local
#include <Std++/Debug.hpp>

void *StdPlusPlus::MemoryAllocate(uint32 size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void StdPlusPlus::MemoryFree(void *pMem)
{
	HeapFree(GetProcessHeap(), 0, pMem);
}

void *StdPlusPlus::MemoryReallocate(void *pMem, uint32 size)
{
	if(!pMem)
		return MemoryAllocate(size);

	return HeapReAlloc(GetProcessHeap(), 0, pMem, size);
}

void StdPlusPlus::VirtualMemoryProtect(void *pMemoryRegion, uint32 size, EVirtualMemoryProtection protection)
{
	DWORD newProtect, oldProtect;

	switch(protection)
	{
	case EVirtualMemoryProtection::Execute_Read:
		newProtect = PAGE_EXECUTE_READ;
		break;
	case EVirtualMemoryProtection::Execute_Read_Write:
		newProtect = PAGE_EXECUTE_READWRITE;
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}

	VirtualProtect(pMemoryRegion, size, newProtect, &oldProtect);
}