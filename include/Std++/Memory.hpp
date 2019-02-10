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
//Local
#include "__Globaldependencies.h"
#include "Definitions.h"
//Definitions
#ifdef _DEBUG
#define MemAlloc(size) MemAllocDebug(size, __FILE__, __LINE__)
#define MemAllocAligned(size, alignment) MemAllocAlignedDebug(size, alignment, __FILE__, __LINE__)
#define MemFree(ptr) MemFreeDebug(ptr)
#define MemRealloc(ptr, size) MemReallocDebug(ptr, size, __FILE__, __LINE__)
#else
#define MemAlloc(size) MemoryAllocate(size)
#define MemAllocAligned(size, alignment) MemAllocAlignedDebug(size, alignment)
#define MemFree(ptr) MemoryFree(ptr)
#define MemRealloc(ptr, size) MemoryReallocate(ptr, size)
#endif

namespace StdXX
{
    enum class MemoryProtection
    {
        Execute_Read,
        Execute_Read_Write
    };

    const uint16 c_guaranteedMemoryAlignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

    //Functions
	inline void *AlignMemoryAddress(void *p, uint8 alignment)
	{
		uint8 offset = static_cast<uint8>(alignment - ((alignment - 1u) & (uint64)p));
		return (uint8 *)p + offset;
	}

#ifdef _DEBUG
	STDPLUSPLUS_API void DebugCheckHeapIntegrity();
    STDPLUSPLUS_API bool DebugDumpMemoryLeaks();
	STDPLUSPLUS_API void *MemAllocDebug(uint32 size, const char *fileName, uint32 lineNumber);
	STDPLUSPLUS_API void MemFreeDebug(void *memBlock);
	STDPLUSPLUS_API void *MemReallocDebug(void *pMem, uint32 size, const char *fileName, uint32 lineNumber);

	inline void *MemAllocAlignedDebug(uint32 size, uint8 alignment, const char *fileName, uint32 lineNumber)
	{
		void *p = MemAllocDebug(size + alignment-1 + 1, fileName, lineNumber); //max error = alignment-1 + one offset byte
		uint8 *p_aligned = static_cast<uint8 *>(AlignMemoryAddress(p, alignment));
		*(p_aligned - 1) = static_cast<uint8>(p_aligned - (uint8 *)p); //write the offset to the unaligned memory location

		return p_aligned;
	}
#endif
    STDPLUSPLUS_API void *MemoryAllocate(uint32 size);
    STDPLUSPLUS_API void MemoryFree(void *pMem);
	STDPLUSPLUS_API void MemoryProtect(void *pMemoryRegion, uint32 size, MemoryProtection protection);
    STDPLUSPLUS_API void *MemoryReallocate(void *pMem, uint32 size);
	STDPLUSPLUS_API void* VirtualMemoryAllocate(uint32 size, MemoryProtection protection);
	STDPLUSPLUS_API void VirtualMemoryFree(void* addr, uint32 size);

#ifndef _DEBUG
	inline void *MemAllocAligned(uint32 size, uint8 alignment)
	{
		void *p = MemoryAllocate(size + alignment-1 + 1); //max error = alignment-1 + one offset byte
		uint8 *p_aligned = static_cast<uint8 *>(AlignMemoryAddress(p, alignment));
		*(p_aligned - 1) = static_cast<uint8>(p_aligned - (uint8 *)p); //write the offset to the unaligned memory location

		return p_aligned;
	}
#endif

    //Inline
    inline int32 MemCmp(const void *pSrc1, const void *pSrc2, uint32 size)
    {
        int32 difference = 0;
        byte *pSrc1Current = (byte *)pSrc1;
        byte *pSrc2Current = (byte *)pSrc2;

        while(size-- && !difference)
            difference = *(pSrc1Current++) - *(pSrc2Current++);

        return difference;
    }

    inline void MemCopy(void *pDest, const void *pSrc, uint32 size)
    {
        byte *pDest1 = (byte *)pDest;
        byte *pSrc1 = (byte *)pSrc;

        //align on 8 bytes
        while(size & 0x7)
        {
            *pDest1++ = *pSrc1++;
            size--;
        }

        //copy 8 bytes based
        uint64 *pDest8 = (uint64 *)pDest1;
        uint64 *pSrc8 = (uint64 *)pSrc1;

        while(size > 32)
        {
            pDest8[0] = pSrc8[0];
            pDest8[1] = pSrc8[1];
            pDest8[2] = pSrc8[2];
            pDest8[3] = pSrc8[3];

            pDest8 += 4;
            pSrc8 += 4;
            size -= 32;
        }

        while(size)
        {
            *pDest8++ = *pSrc8++;
            size -= 8;
        }
    }

	inline void MemFreeAligned(void *pMem)
	{
		uint8 offset = *((byte *)pMem - 1);

		MemFree(((byte *)pMem) - offset);
	}

    inline void MemSet(void *pDest, byte value, uint32 size)
    {
        byte *pCurrent = (byte *)pDest;

        while(size--)
            *pCurrent++ = value;
    }

    inline void MemZero(void *pDest, uint32 size)
    {
        MemSet(pDest, 0, size);
    }
}

//Override global new/delete
#ifdef _DEBUG
extern STDPLUSPLUS_API const char *__file__;
extern STDPLUSPLUS_API int __line__;

#endif
void *operator new(size_t size);
void *operator new[](size_t size);
void *operator new(size_t size, std::align_val_t al);
void *operator new[](size_t size, std::align_val_t al);
void operator delete(void *p) noexcept;
void operator delete[](void *p) noexcept;
void operator delete(void *p, std::align_val_t al) noexcept;
void operator delete[](void *p, std::align_val_t al) noexcept;
void operator delete(void *p, size_t size) noexcept;
void operator delete[](void *p, size_t size) noexcept;

//Placement-new
#define pnew(ptr) new(ptr)

#ifdef _DEBUG
#define new (__file__ = __FILE__, __line__ = __LINE__) && 0 ? NULL : new
#endif