/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
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
#define MemFree(ptr) MemoryFree(ptr)
#define MemRealloc(ptr, size) MemoryReallocate(ptr, size)
#endif

namespace ACStdLib
{
    enum class EVirtualMemoryProtection
    {
        Execute_Read,
        Execute_Read_Write
    };

    //Functions
#ifdef _DEBUG
    ACSTDLIB_API bool DebugDumpMemoryLeaks();
	ACSTDLIB_API void *MemAllocDebug(uint32 size, const char *pFileName, uint32 lineNumber);
	ACSTDLIB_API void MemFreeDebug(void *pMem);
	ACSTDLIB_API void *MemReallocDebug(void *pMem, uint32 size, const char *pFileName, uint32 lineNumber);

	//Inline
	inline void *MemAllocAlignedDebug(uint32 size, uint8 alignment, const char *pFileName, uint32 lineNumber)
	{
		void *p = MemAllocDebug(size + alignment, pFileName, lineNumber); //max error = alignment-1 + one offset byte
		uint8 offset = alignment - ((alignment - 1) & (uint64)p);
		*((byte *)p + offset - 1) = offset; //write the offset to the unaligned memory location

		return (byte *)p + offset;
	}
#endif
    ACSTDLIB_API void *MemoryAllocate(uint32 size);
    ACSTDLIB_API void MemoryFree(void *pMem);
    ACSTDLIB_API void *MemoryReallocate(void *pMem, uint32 size);
    ACSTDLIB_API void VirtualMemoryProtect(void *pMemoryRegion, uint32 size, EVirtualMemoryProtection protection);

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
extern ACSTDLIB_API const char *__file__;
extern ACSTDLIB_API int __line__;

inline void *operator new(size_t size)
{
	return ACStdLib::MemAllocDebug((uint32)size, __file__, __line__);
}
#endif

inline void operator delete(void *p)
{
    ACStdLib::MemFree(p);
}

//Placement-new
#define pnew(ptr) new(ptr)
#ifdef _MSC_VER //in Visual Studio we can define placement-new instead of including <new>
inline void *operator new(size_t, void *pMem)
{
    return pMem;
}

inline void operator delete(void *, void *)
{
}
#endif

#ifdef _DEBUG
#define new (__file__ = __FILE__, __line__ = __LINE__) && 0 ? NULL : new
#endif