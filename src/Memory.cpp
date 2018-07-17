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
#include <Std++/Memory.h>
#ifdef _DEBUG
//Global
#include <stdio.h>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Streams/StdOut.hpp>
//Namespaces
using namespace StdPlusPlus;
//Definitions
#define HEAP_BLOCK_IDENTIFIER u8"Std++MBH" //MemoryBlockHeader
#define HEAP_BLOCK_IDENTIFIER_SIZE 8
#define HEAP_CORRUPTION_DETECTIONSECTION_SIZE 12
#define HEAP_CORRUPTION_DETECTIONSECTION_VALUE 0xFD
#define HEAP_INIT_VALUE 0xCD

//this is so ugly... but we need a mutex that doesnt take dynamic memory
//or else the lock() calls will fail
#ifdef XPC_OS_WINDOWS
#include <Windows.h>
class InternalMutex
{
public:
	CRITICAL_SECTION cs;

	inline InternalMutex()
	{
		InitializeCriticalSection(&cs);
	}

	inline ~InternalMutex()
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
#elif defined(XPC_OS_LINUX) || defined(XPC_OS_DARWIN)
#include <pthread.h>
class InternalMutex
{
public:
	pthread_mutex_t mutex;

	inline InternalMutex()
	{
		pthread_mutex_init(&mutex, nullptr);
	}

	inline ~InternalMutex()
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
//The shared global mutex
static InternalMutex g_memMutex;



struct DebugMemBlockHeader
{
	byte identifier[HEAP_BLOCK_IDENTIFIER_SIZE];
    DebugMemBlockHeader *pPrev;
    DebugMemBlockHeader *pNext;
    const char *pFileName;
    uint32 lineNumber;
    uint32 userSize;
    uint32 seqNumber;
    byte preHeapCorriptionDetectionSection[HEAP_CORRUPTION_DETECTIONSECTION_SIZE];

    //Inline
	inline byte *GetPostHeapCorruptionDetectionAddress() const
	{
		//the region is right after the user data
		return this->GetUserData() + this->userSize;
	}

	inline byte *GetUserData() const
	{
		//after the header is the user data
		return (byte *) (this + 1);
	}

	inline bool IsCorrupt() const
	{
		//check identifier
		if (MemCmp(this->identifier, HEAP_BLOCK_IDENTIFIER, sizeof(this->identifier)))
			return true;
		//check heap corruption detection section before user memory
		if(!this->CheckBytes(this->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, sizeof(this->preHeapCorriptionDetectionSection)))
			return true;
		//check heap corruption detection section after user memory
		if(!this->CheckBytes(this->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
			return true;

		return false;
	}

	inline void VerifyIntegrity() const
	{
		if(this->IsCorrupt())
		{
			g_memMutex.Unlock(); //we need to free the lock so that ASSERT can allocate
			ASSERT(false, u8"HEAP CORRUPTED. Check memory dump!");
			g_memMutex.Lock();
		}
	}

private:
	//Inline
	inline bool CheckBytes(const void *bytes, byte mustBeValue, uint32 size) const
	{
		byte *ptr = (byte *)bytes;
		while(size--)
			if(*ptr++ != mustBeValue)
				return false;

		return true;
	}
};


/*
We manage memory like this:
DebugMemBlockHeader that includes heap corruption detection section; user memory; heap corruption detection section
*/

//Global Variables
static uint32 g_seqNumber = 1;
static uint32 g_seqNumberUser = 1;
static DebugMemBlockHeader *g_pFirstMemBlock = nullptr;
static DebugMemBlockHeader *g_pLastMemBlock = nullptr;

//Local Functions
static DebugMemBlockHeader *GetHeaderFromUserData(const void *userData)
{
	return reinterpret_cast<DebugMemBlockHeader *>(((byte *)userData) - sizeof(DebugMemBlockHeader));
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

//Namespace Functions
void StdPlusPlus::DebugCheckHeapIntegrity()
{
	g_memMutex.Lock();

	for (DebugMemBlockHeader *block = g_pFirstMemBlock; block; block = block->pNext)
	{
		block->VerifyIntegrity();
	}

	g_memMutex.Unlock();
}

bool StdPlusPlus::DebugDumpMemoryLeaks()
{
    bool hasLeaks;
    DebugMemBlockHeader *pBlock;
    FILE *fp;

    g_memMutex.Lock(); //VERY IMPORTANT! From now on, no allocation whatsoever is allowed to be made using Std++::MemAllocDebug or the other Std++ Memory Debug functions

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

    //fp = fopen("StdPlusPlus MemLeaks.txt", "w"); //we use the standard library here in order to not generate any memory allocations
    fp = stderr;

	fprintf(fp, "!!!YOU HAVE MEMORY LEAKS!!!\r\n");
    fprintf(fp, "Memory blocks that leak:\r\n");
    while(pBlock)
    {
        if(pBlock->seqNumber >= g_seqNumberUser)
        {
            fprintf(fp, "Block %d, Size: %d bytes, Allocated in: %s:%d\r\n", pBlock->seqNumber, pBlock->userSize, pBlock->pFileName, pBlock->lineNumber);

            if(pBlock->IsCorrupt())
            {
                fprintf(fp, "HEAP CORRUPTED. Memory before or/and after block was modified. Value should be: ");
                DumpByte(HEAP_CORRUPTION_DETECTIONSECTION_VALUE, fp);
                fprintf(fp, "\r\n");
            }

            fprintf(fp, "Dump: \r\n");

            //heap corruption before data
            if(pBlock->IsCorrupt())
            {
                fprintf(fp, "\tBefore block:\t");
                DumpBytes(pBlock->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_SIZE, fp);
                fprintf(fp, "\r\n");
            }

            //data
            fprintf(fp, "\tUser data:\t");
            DumpBytes(pBlock->GetUserData(), pBlock->userSize, fp);
            fprintf(fp, "\r\n\tASCII:\t\t");
            DumpASCII(pBlock->GetUserData(), pBlock->userSize, fp);
            fprintf(fp, "\r\n");

            //heap corruption after data
            if(pBlock->IsCorrupt())
            {
                fprintf(fp, "\tAfter block:\t");
                DumpBytes(pBlock->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_SIZE, fp);
                fprintf(fp, "\r\n");
            }
        }

        pBlock = pBlock->pNext;
    }

    fclose(fp);

    g_memMutex.Unlock();

    return true;
}

void *StdPlusPlus::MemAllocDebug(uint32 size, const char *fileName, uint32 lineNumber)
{
	g_memMutex.Lock();

    DebugMemBlockHeader *memBlockHeader = (DebugMemBlockHeader *)MemoryAllocate(sizeof(DebugMemBlockHeader) + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    //fill out block header
	MemCopy(memBlockHeader->identifier, HEAP_BLOCK_IDENTIFIER, sizeof(memBlockHeader->identifier));
    memBlockHeader->lineNumber = lineNumber;
    memBlockHeader->pFileName = fileName;
    memBlockHeader->pNext = nullptr;
    memBlockHeader->pPrev = g_pLastMemBlock;
    memBlockHeader->seqNumber = g_seqNumber++;
    memBlockHeader->userSize = size;

    //link block
    if(g_pLastMemBlock)
        g_pLastMemBlock->pNext = memBlockHeader;
    else
        g_pFirstMemBlock = memBlockHeader;
    g_pLastMemBlock = memBlockHeader;

    //fill out the memory corruption detection sections
	MemSet(memBlockHeader->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, sizeof(memBlockHeader->preHeapCorriptionDetectionSection));
    MemSet(memBlockHeader->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    //init memory with something else than 0
    MemSet(memBlockHeader->GetUserData(), HEAP_INIT_VALUE, size);

    g_memMutex.Unlock();

    return memBlockHeader->GetUserData();
}

void StdPlusPlus::MemFreeDebug(void *userData)
{
	g_memMutex.Lock();

	DebugMemBlockHeader *memBlockHeader = GetHeaderFromUserData(userData);
	memBlockHeader->VerifyIntegrity();

    //remove from list
    if(memBlockHeader->pNext)
    {
        memBlockHeader->pNext->pPrev = memBlockHeader->pPrev;
    }
    else
    {
        ASSERT(g_pLastMemBlock == memBlockHeader, u8"If you see this, report to StdPlusPlus");
        g_pLastMemBlock = memBlockHeader->pPrev;
    }

    if(memBlockHeader->pPrev)
    {
        memBlockHeader->pPrev->pNext = memBlockHeader->pNext;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == memBlockHeader, u8"If you see this, report to StdPlusPlus");
        g_pFirstMemBlock = memBlockHeader->pNext;
    }

    //clear whole memory with a random value (but not 0)
    //this option invalidates memBlockHeader!!!
    MemSet(memBlockHeader, 0xDD, sizeof(DebugMemBlockHeader) + memBlockHeader->userSize + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    MemoryFree(memBlockHeader);

    g_memMutex.Unlock();
}

void *StdPlusPlus::MemReallocDebug(void *pMem, uint32 size, const char *pFileName, uint32 lineNumber)
{
    if(!pMem)
        return MemAllocDebug(size, pFileName, lineNumber);

    g_memMutex.Lock();

    //fetch block memory header
	DebugMemBlockHeader *oldBlock = GetHeaderFromUserData(pMem);
    oldBlock->VerifyIntegrity();

    //do reallocate
	DebugMemBlockHeader *newBlock = (DebugMemBlockHeader *)MemoryReallocate(oldBlock, sizeof(DebugMemBlockHeader) + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    ASSERT(newBlock, u8"If you see this, report to StdPlusPlus");

    if(size > newBlock->userSize)
    {
        //fill new allocated memory with random value
        MemSet((byte *)pMem + newBlock->userSize, HEAP_INIT_VALUE, size - newBlock->userSize);
    }

	//update header
	newBlock->pFileName = pFileName;
	newBlock->lineNumber = lineNumber;
	newBlock->seqNumber = g_seqNumber++;
	newBlock->userSize = size;

    //fill out the memory corruption detection section after user memory
    MemSet(newBlock->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    if(oldBlock == newBlock)
    {
        //nothing to be done
        g_memMutex.Unlock();
        return newBlock->GetUserData();
    }

    //memory has been moved...
    //new block header pointers are still correct...
    //but old block is still referenced
    if(newBlock->pNext)
    {
        newBlock->pNext->pPrev = newBlock->pPrev;
    }
    else
    {
        ASSERT(g_pLastMemBlock == oldBlock, u8"If you see this, report to StdPlusPlus");
        g_pLastMemBlock = newBlock->pPrev;
    }

    if(newBlock->pPrev)
    {
        newBlock->pPrev->pNext = newBlock->pNext;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == oldBlock, u8"If you see this, report to StdPlusPlus");
        g_pFirstMemBlock = newBlock->pNext;
    }

    //put new block to the end of the list
    if(g_pLastMemBlock)
        g_pLastMemBlock->pNext = newBlock;
    else
        g_pFirstMemBlock = newBlock;

    newBlock->pPrev = g_pLastMemBlock;
    newBlock->pNext = NULL;

    g_pLastMemBlock = newBlock;

    g_memMutex.Unlock();
    return newBlock->GetUserData();
}

STDPLUSPLUS_API void StartUserMemoryLogging()
{
    g_seqNumberUser = g_seqNumber;
}

#undef new
void *operator new(size_t size)
{
	const char *fileName = __file__;
	int lineNumber = __line__;

	__file__ = u8"???";
	__line__ = -1;

	return StdPlusPlus::MemAllocDebug((uint32)size, fileName, lineNumber);
}

const char *__file__;
int __line__;
#else
void *operator new(size_t size)
{
	return StdPlusPlus::MemoryAllocate(size);
}
#endif

void operator delete(void *p) noexcept
{
	StdPlusPlus::MemFree(p);
}