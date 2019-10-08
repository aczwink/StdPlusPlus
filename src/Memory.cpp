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
//Local
#include <Std++/Mathematics.hpp>

//Namespace functions
void StdXX::MemMove(void *destination, const void *source, uint32 size)
{
	const uint8* src = static_cast<const uint8 *>(source);
	uint8* dest = static_cast<uint8 *>(destination);

	if(Math::IsValueInInterval((const uint8*)dest, src, src+size))
	{
		//overlap, copy from end
		src += size - 1;
		dest += size - 1;
		while(size--)
		{
			*dest-- = *src--;
		}
		return;
	}

	MemCopy(destination, source, size);
}


#ifdef XPC_BUILDTYPE_DEBUG
//Global
#include <stdio.h>
#include <new>
//Local
#include <Std++/Debug.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Streams/Writers/StdOut.hpp>
//Namespaces
using namespace StdXX;
//Definitions
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

/*
 * We manage memory like this:
 * DebugMemBlockHeader that includes heap corruption detection section; user memory; heap corruption detection section
 *
 * !!!
 * We must be extremely careful to not break memory alignment by inserting this structure.
 * Else we will trigger lots of exceptions.
 * We can variate the alignment with the heap corrution detection size.
 * !!!
 */
struct DebugMemBlockHeader
{
    DebugMemBlockHeader *previous;
    DebugMemBlockHeader *next;
    const char *pFileName;
    uint32 lineNumber;
    uint32 userSize;
    uint32 seqNumber;
    uint32 alignmentOffset;
    byte preHeapCorriptionDetectionSection[HEAP_CORRUPTION_DETECTIONSECTION_SIZE];

    //Inline
	inline void *GetAllocatedMemoryAddress() const
	{
		return ((byte *)this) - this->alignmentOffset;
	}

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

//Global Variables
static uint32 g_seqNumber = 1;
static uint32 g_seqNumberUser = 1;
static DebugMemBlockHeader *g_pFirstMemBlock = nullptr;
static DebugMemBlockHeader *g_pLastMemBlock = nullptr;

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
void StdXX::DebugCheckHeapIntegrity()
{
	g_memMutex.Lock();

	for (DebugMemBlockHeader *block = g_pFirstMemBlock; block; block = block->next)
	{
		block->VerifyIntegrity();
	}

	g_memMutex.Unlock();
}

bool StdXX::DebugDumpMemoryLeaks()
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

        pBlock = pBlock->next;
    }
    if(!hasLeaks)
    {
        g_memMutex.Unlock();
        return false;
    }

    //fp = fopen("StdXX MemLeaks.txt", "w"); //we use the standard library here in order to not generate any memory allocations
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

        pBlock = pBlock->next;
    }

    fclose(fp);

    g_memMutex.Unlock();

    return true;
}

void *StdXX::MemAllocDebug(uint32 size, const char *fileName, uint32 lineNumber)
{
	g_memMutex.Lock();

	byte *memoryBlock = (byte *)MemoryAllocate(sizeof(DebugMemBlockHeader) + c_guaranteedMemoryAlignment + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
	//memoryBlock is aligned, make sure that user memory is also aligned, then put header directly before that
	DebugMemBlockHeader *memBlockHeader = (DebugMemBlockHeader *)((uint8 *)AlignMemoryAddress(memoryBlock + sizeof(DebugMemBlockHeader), c_guaranteedMemoryAlignment) - sizeof(DebugMemBlockHeader));

    //fill out block header
    memBlockHeader->lineNumber = lineNumber;
    memBlockHeader->pFileName = fileName;
    memBlockHeader->next = nullptr;
    memBlockHeader->previous = g_pLastMemBlock;
    memBlockHeader->seqNumber = g_seqNumber++;
    memBlockHeader->userSize = size;
    memBlockHeader->alignmentOffset = static_cast<uint32>(((byte *)memBlockHeader) - memoryBlock);

    //link block
    if(g_pLastMemBlock)
        g_pLastMemBlock->next = memBlockHeader;
    else
        g_pFirstMemBlock = memBlockHeader;
    g_pLastMemBlock = memBlockHeader;

    //fill out the memory corruption detection sections
	MemSet(memBlockHeader->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, sizeof(memBlockHeader->preHeapCorriptionDetectionSection));
    MemSet(memBlockHeader->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    //init memory with something else than 0
    MemSet(memBlockHeader->GetUserData(), HEAP_INIT_VALUE, size);

    void *userData = memBlockHeader->GetUserData();

    g_memMutex.Unlock();

    return userData;
}

void StdXX::MemFreeDebug(void *userData)
{
	if (userData == nullptr)
		return;

	g_memMutex.Lock();

	DebugMemBlockHeader *memBlockHeader = GetHeaderFromUserData(userData);
	memBlockHeader->VerifyIntegrity();

    //remove from list
    if(memBlockHeader->next)
    {
        memBlockHeader->next->previous = memBlockHeader->previous;
    }
    else
    {
        ASSERT(g_pLastMemBlock == memBlockHeader, u8"If you see this, report to StdXX");
        g_pLastMemBlock = memBlockHeader->previous;
    }

    if(memBlockHeader->previous)
    {
        memBlockHeader->previous->next = memBlockHeader->next;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == memBlockHeader, u8"If you see this, report to StdXX");
        g_pFirstMemBlock = memBlockHeader->next;
    }

    void *memBlock = memBlockHeader->GetAllocatedMemoryAddress();

    //clear whole memory with a random value (but not 0)
    //this option invalidates memBlockHeader!!!
    MemSet(memBlockHeader, 0xDD, sizeof(DebugMemBlockHeader) + memBlockHeader->userSize + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    MemoryFree(memBlock);

    g_memMutex.Unlock();
}

void *StdXX::MemReallocDebug(void *pMem, uint32 size, const char *fileName, uint32 lineNumber)
{
    if(!pMem)
        return MemAllocDebug(size, fileName, lineNumber);

    g_memMutex.Lock();

    //fetch block memory header
	DebugMemBlockHeader *oldMemHeader = GetHeaderFromUserData(pMem);
    oldMemHeader->VerifyIntegrity();

    //do reallocate
	byte *memoryBlock = (byte *)MemoryReallocate(oldMemHeader->GetAllocatedMemoryAddress(), sizeof(DebugMemBlockHeader) + c_guaranteedMemoryAlignment + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
	//memoryBlock is aligned, make sure that user memory is also aligned, then put header directly before that
	DebugMemBlockHeader *newBlock = (DebugMemBlockHeader *)((uint8 *)AlignMemoryAddress(memoryBlock + sizeof(DebugMemBlockHeader), c_guaranteedMemoryAlignment) - sizeof(DebugMemBlockHeader));
    ASSERT(newBlock, u8"If you see this, report to StdXX");

    if(size > newBlock->userSize)
    {
        //fill new allocated memory with random value
        MemSet(newBlock->GetUserData() + newBlock->userSize, HEAP_INIT_VALUE, size - newBlock->userSize);
    }

	//update header
	newBlock->pFileName = fileName;
	newBlock->lineNumber = lineNumber;
	newBlock->seqNumber = g_seqNumber++;
	newBlock->userSize = size;
	newBlock->alignmentOffset = static_cast<uint32>(((byte *)newBlock) - memoryBlock);

    //fill out the memory corruption detection section after user memory
    MemSet(newBlock->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    if(oldMemHeader == newBlock)
    {
        //nothing to be done
        g_memMutex.Unlock();
        return newBlock->GetUserData();
    }

    //memory has been moved...
    //new block header pointers are still correct...
    //but old block is still referenced
    if(newBlock->next)
    {
        newBlock->next->previous = newBlock->previous;
    }
    else
    {
        ASSERT(g_pLastMemBlock == oldMemHeader, u8"If you see this, report to StdXX");
        g_pLastMemBlock = newBlock->previous;
    }

    if(newBlock->previous)
    {
        newBlock->previous->next = newBlock->next;
    }
    else
    {
        ASSERT(g_pFirstMemBlock == oldMemHeader, u8"If you see this, report to StdXX");
        g_pFirstMemBlock = newBlock->next;
    }

    //put new block to the end of the list
    if(g_pLastMemBlock)
        g_pLastMemBlock->next = newBlock;
    else
        g_pFirstMemBlock = newBlock;

    newBlock->previous = g_pLastMemBlock;
    newBlock->next = NULL;

    g_pLastMemBlock = newBlock;

    void *userData = newBlock->GetUserData();
    g_memMutex.Unlock();

    return userData;
}

STDPLUSPLUS_API void StartUserMemoryLogging()
{
    g_seqNumberUser = g_seqNumber;
}
#endif