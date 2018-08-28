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

#ifdef _DEBUG
#undef new
void *operator new(size_t size)
{
	const char *fileName = __file__;
	__file__ = u8"???";

	int lineNumber = __line__;
	__line__ = -1;

	return StdXX::MemAllocDebug((uint32)size, fileName, lineNumber);
}

void *operator new[](size_t size)
{
	const char *fileName = __file__;
	__file__ = u8"???";

	int lineNumber = __line__;
	__line__ = -1;

	return StdXX::MemAllocDebug((uint32)size, fileName, lineNumber);
}

void *operator new(size_t size, std::align_val_t al)
{
	const char *fileName = __file__;
	__file__ = u8"???";

	int lineNumber = __line__;
	__line__ = -1;

	return StdXX::MemAllocAlignedDebug((uint32)size, (uint8)al, fileName, lineNumber);
}

void *operator new[](size_t size, std::align_val_t al)
{
	const char *fileName = __file__;
	__file__ = u8"???";

	int lineNumber = __line__;
	__line__ = -1;

	return StdXX::MemAllocAlignedDebug((uint32)size, (uint8)al, fileName, lineNumber);
}

const char *__file__;
int __line__;
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