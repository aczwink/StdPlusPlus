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
 //corresponding header
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX::Memory;

#ifdef XPC_BUILDTYPE_DEBUG
#undef new
void *operator new(size_t size)
{
	void* ptr = MemoryManager::GlobalAllocator().Allocate(size);
	_stdxx_::TrySetMemoryContext("???", -1);
	return ptr;
}

void *operator new[](size_t size)
{
	void* ptr = MemoryManager::GlobalAllocator().Allocate(size);
	_stdxx_::TrySetMemoryContext("???", -1);
	return ptr;
}

void *operator new(size_t size, std::align_val_t al)
{
	uint8 alignment = (uint8)al;
	void* ptr = MemoryManager::GlobalAllocator().Allocate(_stdxx_::ComputeSizeWithAlignment(size, alignment));
	_stdxx_::TrySetMemoryContext("???", -1);
	return _stdxx_::SaveAlignmentOffsetAndAlignMemory(ptr, alignment);
}

void *operator new[](size_t size, std::align_val_t al)
{
	uint8 alignment = (uint8)al;
	void* ptr = MemoryManager::GlobalAllocator().Allocate(_stdxx_::ComputeSizeWithAlignment(size, alignment));
	_stdxx_::TrySetMemoryContext("???", -1);
	return _stdxx_::SaveAlignmentOffsetAndAlignMemory(ptr, alignment);
}

#else
void *operator new(size_t size)
{
	return StdXX::MemoryAllocate(size);
}
#endif

void operator delete(void *p) noexcept
{
	StdXX::MemFree(p);
}

void operator delete[](void *p) noexcept
{
	StdXX::MemFree(p);
}

void operator delete(void *p, size_t) noexcept
{
	StdXX::MemFree(p);
}

void operator delete[](void *p, size_t) noexcept
{
	StdXX::MemFree(p);
}

void operator delete(void *p, std::align_val_t al) noexcept
{
	StdXX::MemFreeAligned(p);
}

void operator delete[](void *p, std::align_val_t al) noexcept
{
	StdXX::MemFreeAligned(p);
}