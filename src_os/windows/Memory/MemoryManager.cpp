/*
* Copyright (c) 2017-2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Windows.h>
//Namespaces
using namespace StdXX::Memory;

class WindowsProcessAllocator : public Allocator
{
public:
	//Constructor
	inline WindowsProcessAllocator()
	{
		this->processHeap = GetProcessHeap();
	}

	//Methods
	virtual void* Allocate(uint32 size) override
	{
		return HeapAlloc(this->processHeap, 0, size);
	}

	virtual void Free(void* mem) override
	{
		HeapFree(this->processHeap, 0, mem);
	}

	virtual void* Reallocate(void* mem, uint32 size) override
	{
		if (!mem)
			return this->Allocate(size);

		return HeapReAlloc(this->processHeap, 0, mem, size);
	}

private:
	//Members
	HANDLE processHeap;
};

//Public methods
Allocator& MemoryManager::ProcessSystemAllocator()
{
	static WindowsProcessAllocator wpa;
	return wpa;
}
