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
#ifdef _DEBUG
//corresponding header
#include <ACStdLib/Memory.h>
//Global
#include <stdio.h>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Streams/CFileOutputStream.h>
#include <ACStdLib/Streams/CStdOut.h>
//Namespaces
using namespace ACStdLib;
//Definitions
#define HEAP_CORRUPTION_DETECTIONSECTION_SIZE 12
#define HEAP_CORRUPTION_DETECTIONSECTION_VALUE 0xFD
#define HEAP_INIT_VALUE 0xCD

struct SDebugMemBlockHeader
{
    SDebugMemBlockHeader *pPrev;
    SDebugMemBlockHeader *pNext;
    const char *pFileName;
    uint32 lineNumber;
    uint32 userSize;
    uint32 seqNumber;
};

//this is so ugly... but we need a mutex that doesnt take dynamic memory
//or else the lock() calls will fail
#ifdef _AC_OS_WINDOWS
#include <Windows.h>
class CInternalMutex
{
public:
	CRITICAL_SECTION cs;

	inline CInternalMutex()
	{
		InitializeCriticalSection(&cs);
	}

	inline ~CInternalMutex()
	{
		DeleteCriticalSection(&cs);
	}

	inline void Lock()
	{
		EnterCriticalSection(&cs);
	}

	inline void Unlock()
	{
		LeaveCriticalSection(&cs);
	}
};
#endif
#ifdef _AC_OS_LINUX
#include <pthread.h>
class CInternalMutex
{
public:
    pthread_mutex_t mutex;

	inline CInternalMutex()
	{
        pthread_mutex_init(&mutex, nullptr);
	}

	inline ~CInternalMutex()
	{
        pthread_mutex_destroy(&mutex);
	}

	inline void Lock()
	{
        pthread_mutex_lock(&mutex);
	}

	inline void Unlock()
	{
        pthread_mutex_unlock(&mutex);
	}
};
#endif


/*
We manage memory like this:
SDebugMemBlockHeader, heap corruption detection section; user memory; heap corruption detection section
*/

//Global Variables
static uint32 g_seqNumber = 1;
static uint32 g_seqNumberUser = 1;
static SDebugMemBlockHeader *g_pFirstMemBlock = NULL;
static SDebugMemBlockHeader *g_pLastMemBlock = NULL;
static CInternalMutex g_memMutex;

//Local Functions
static bool CheckBytes(const void *pBytes, byte mustBeValue, uint32 size)
{
    byte *ptr;

    ptr = (byte *)pBytes;
    while(size--)
        if(*ptr++ != mustBeValue)
            return false;

    return true;
}

inline void DumpByte(byte b, FILE *fp)
{
    char c1, c2;

    c1 = b >> 4;
    c2 = b & 0xF;

    if(c1 >= 10)
        c1 = c1 - 10 + 'A';
    else
        c1 += '0';
    if(c2 >= 10)
        c2 = c2 - 10 + 'A';
    else
        c2 += '0';

    fputc(c1, fp);
    fputc(c2, fp);
}

static void DumpASCII(const void *pMem, uint32 size, FILE *fp)
{
    bool dots;
    byte *pMemBytes;

    dots = false;
    if(size > 20)
    {
        size = 20;
        dots = true;
    }

    pMemBytes = (byte *)pMem;
    for(uint32 i = 0; i < size; i++)
    {
        fputc(' ', fp);
        if(*pMemBytes >= 32 && *pMemBytes <= 127)
            fputc((char)*pMemBytes, fp);
        else
            fputc(' ', fp);
        pMemBytes++;
        fputc(' ', fp);
    }

    if(dots)
        fprintf(fp, "...");
}

static void DumpBytes(const void *pMem, uint32 size, FILE *fp)
{
    bool dots;
    byte *pMemBytes;

    dots = false;
    if(size > 20)
    {
        size = 20;
        dots = true;
    }

    pMemBytes = (byte *)pMem;
    for(uint32 i = 0; i < size; i++)
    {
        DumpByte(*pMemBytes++, fp);
        fputc(' ', fp);
    }

    if(dots)
        fprintf(fp, "...");
}

inline byte *GetUserData(SDebugMemBlockHeader *pBlockHdr)
{
    //after the header is the corruption detection and then the user data
    return ((byte *)(pBlockHdr + 1)) + HEAP_CORRUPTION_DETECTIONSECTION_SIZE;
}

inline byte *GetFirstHeapCorruptionDetectionAddress(SDebugMemBlockHeader *pBlockHdr)
{
    //the region is right after the header
    return (byte *)(pBlockHdr + 1);
}

inline byte *GetSecondHeapCorruptionDetectionAddress(SDebugMemBlockHeader *pBlockHdr)
{
    //the region is right after the user data
    return GetUserData(pBlockHdr) + pBlockHdr->userSize;
}

//Namespace Functions
bool ACStdLib::DebugDumpMemoryLeaks()
{
    bool hasLeaks, heapCorrupt;
    byte *pCorrupt1, *pCorrupt2;
    SDebugMemBlockHeader *pBlock;
    FILE *fp;

    g_memMutex.Lock(); //VERY IMPORTANT! From now on, no allocation whatsoever is allowed to be made using ACStdLib::MemAllocDebug or the other ACStdLib Memory Debug functions

    hasLeaks = false;
    pBlock = g_pFirstMemBlock;
    while(pBlock)
    {
        if(pBlock->seqNumber >= g_seqNumberUser) //very ugly but we need to exclude all global objects that are destroyed AFTER the library gets unloaded
        {
            hasLeaks = true;
            break;
        }

        pBlock = pBlock->pNext;
    }
    if(!hasLeaks)
    {
        g_memMutex.Unlock();
        return false;
    }

    //fp = fopen("ACStdLib MemLeaks.txt", "w"); //we use the standard library here in order to not generate any memory allocations
    fp = stderr;

    fprintf(fp, "Memory leaks:\r\n");
    while(pBlock)
    {
        heapCorrupt = false;

        if(pBlock->seqNumber >= g_seqNumberUser)
        {
            //check heap corruption detection section before user memory
            pCorrupt1 = GetFirstHeapCorruptionDetectionAddress(pBlock);
            if(!CheckBytes(pCorrupt1, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
            {
                heapCorrupt = true;
            }

            //check heap corruption detection section after user memory
            pCorrupt2 = GetSecondHeapCorruptionDetectionAddress(pBlock);
            if(!heapCorrupt && !CheckBytes(pCorrupt2, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
            {
                heapCorrupt = true;
            }

            fprintf(fp, "Block %d, Size: %d bytes, Allocated in: %s:%d\r\n", pBlock->seqNumber, pBlock->userSize, pBlock->pFileName, pBlock->lineNumber);

            if(heapCorrupt)
            {
                fprintf(fp, "HEAP CORRUPTED. Memory before or/and after block was modified. Value should be: ");
                DumpByte(HEAP_CORRUPTION_DETECTIONSECTION_VALUE, fp);
                fprintf(fp, "\r\n");
            }

            fprintf(fp, "Dump: \r\n");

            //heap corruption before data
            if(heapCorrupt)
            {
                fprintf(fp, "\tBefore block:\t");
                DumpBytes(pCorrupt1, HEAP_CORRUPTION_DETECTIONSECTION_SIZE, fp);
                fprintf(fp, "\r\n");
            }

            //data
            fprintf(fp, "\tUser data:\t");
            DumpBytes(GetUserData(pBlock), pBlock->userSize, fp);
            fprintf(fp, "\r\n\tASCII:\t\t");
            DumpASCII(GetUserData(pBlock), pBlock->userSize, fp);
            fprintf(fp, "\r\n");

            //heap corruption after data
            if(heapCorrupt)
            {
                fprintf(fp, "\tAfter block:\t");
                DumpBytes(pCorrupt2, HEAP_CORRUPTION_DETECTIONSECTION_SIZE, fp);
                fprintf(fp, "\r\n");
            }
        }

        pBlock = pBlock->pNext;
    }

    fclose(fp);

    g_memMutex.Unlock();

    return true;
}

void *ACStdLib::MemAllocDebug(uint32 size, const char *pFileName, uint32 lineNumber)
{
    byte *pUserData;
    SDebugMemBlockHeader *pMemBlock;

    g_memMutex.Lock();

    pMemBlock = (SDebugMemBlockHeader *)MemoryAllocate(size + sizeof(SDebugMemBlockHeader) + 2 * HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    pUserData = ((byte *)(pMemBlock + 1)) + HEAP_CORRUPTION_DETECTIONSECTION_SIZE;

    //fill out block header
    pMemBlock->lineNumber = lineNumber;
    pMemBlock->pFileName = pFileName;
    pMemBlock->pNext = NULL;
    pMemBlock->pPrev = g_pLastMemBlock;
    pMemBlock->seqNumber = g_seqNumber++;
    pMemBlock->userSize = size;

    //link block ACStdLib
    if(g_pLastMemBlock)
        g_pLastMemBlock->pNext = pMemBlock;
    else
        g_pFirstMemBlock = pMemBlock;
    g_pLastMemBlock = pMemBlock;

    //fill out the memory corruption detection sections
    MemSet(pMemBlock + 1, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    MemSet(pUserData + size, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    //init memory with something else than 0
    MemSet(pUserData, HEAP_INIT_VALUE, size);

    g_memMutex.Unlock();

    return pUserData;
}

void ACStdLib::MemFreeDebug(void *pMem)
{
    SDebugMemBlockHeader *pMemBlock;

    g_memMutex.Lock();

    pMemBlock = (SDebugMemBlockHeader *)((byte *)pMem - HEAP_CORRUPTION_DETECTIONSECTION_SIZE - sizeof(SDebugMemBlockHeader));

    //check heap corruption detection section before user memory
    if(!CheckBytes(pMemBlock + 1, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
    {
        ASSERT_MSG(false, "HEAP CORRUPTED. Check memory dump!");
    }

    //check heap corruption detection section after user memory
    if(!CheckBytes((byte *)pMem + pMemBlock->userSize, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
    {
        ASSERT_MSG(false, "HEAP CORRUPTED. Check memory dump!");
    }

    //remove from list
    if(pMemBlock->pNext)
    {
        pMemBlock->pNext->pPrev = pMemBlock->pPrev;
    }
    else
    {
        ASSERT(g_pLastMemBlock == pMemBlock);
        g_pLastMemBlock = pMemBlock->pPrev;
    }

    if(pMemBlock->pPrev)
    {
        pMemBlock->pPrev->pNext = pMemBlock->pNext;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == pMemBlock);
        g_pFirstMemBlock = pMemBlock->pNext;
    }

    //clear whole memory with a random value (but not 0)
    //this option invalidates pMemBlock!!!
    MemSet(pMemBlock, 0xDD, pMemBlock->userSize + sizeof(SDebugMemBlockHeader) + 2 * HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    MemoryFree(pMemBlock);

    g_memMutex.Unlock();
}

void *ACStdLib::MemReallocDebug(void *pMem, uint32 size, const char *pFileName, uint32 lineNumber)
{
    SDebugMemBlockHeader *pNewBlock, *pOldBlock;

    if(!pMem)
        return MemAllocDebug(size, pFileName, lineNumber);

    g_memMutex.Lock();

    //fetch block memory header
    pOldBlock = (SDebugMemBlockHeader *)((byte *)pMem - HEAP_CORRUPTION_DETECTIONSECTION_SIZE - sizeof(SDebugMemBlockHeader));

    //do reallocate
    pNewBlock = (SDebugMemBlockHeader *)MemoryReallocate(pOldBlock, size + sizeof(SDebugMemBlockHeader) + 2 * HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    ASSERT(pNewBlock);
    pMem = ((byte *)(pNewBlock + 1)) + HEAP_CORRUPTION_DETECTIONSECTION_SIZE;

    if(size > pNewBlock->userSize)
    {
        //fill new allocated memory with random value
        MemSet((byte *)pMem + pNewBlock->userSize, HEAP_INIT_VALUE, size - pNewBlock->userSize);
    }

    //fill out the memory corruption detection section after user memory
    MemSet((byte *)pMem + size, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    //update header
    pNewBlock->pFileName = pFileName;
    pNewBlock->lineNumber = lineNumber;
    pNewBlock->seqNumber = g_seqNumber++;
    pNewBlock->userSize = size;

    if(pOldBlock == pNewBlock)
    {
        //nothing to be done
        g_memMutex.Unlock();
        return pMem;
    }

    //memory has been moved...
    //new block header pointers are still correct...
    //but old block is still referenced
    if(pNewBlock->pNext)
    {
        pNewBlock->pNext->pPrev = pNewBlock->pPrev;
    }
    else
    {
        ASSERT(g_pLastMemBlock == pOldBlock);
        g_pLastMemBlock = pNewBlock->pPrev;
    }

    if(pNewBlock->pPrev)
    {
        pNewBlock->pPrev->pNext = pNewBlock->pNext;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == pOldBlock);
        g_pFirstMemBlock = pNewBlock->pNext;
    }

    //put new block to the end of the list
    if(g_pLastMemBlock)
        g_pLastMemBlock->pNext = pNewBlock;
    else
        g_pFirstMemBlock = pNewBlock;

    pNewBlock->pPrev = g_pLastMemBlock;
    pNewBlock->pNext = NULL;

    g_pLastMemBlock = pNewBlock;

    g_memMutex.Unlock();
    return pMem;
}

void StartUserMemoryLogging()
{
    g_seqNumberUser = g_seqNumber;
}

const char *__file__;
int __line__;
#endif