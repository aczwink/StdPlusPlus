/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory/MemoryTrackingAllocator.hpp>
//Global
#include <stdio.h>
//Local
#include <Std++/Memory.hpp>
#include "DebugMemBlockHeader.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Memory;
//Definitions
#define HEAP_INIT_VALUE 0xCD

struct Context
{
	const char* fileName;
	uint32 lineNumber;
};
thread_local Context currentContext;

//Local functions
static void DumpASCII(const void* pMem, uint32 size, FILE* fp)
{
    bool dots;
    byte* pMemBytes;

    dots = false;
    if (size > 20)
    {
        size = 20;
        dots = true;
    }

    pMemBytes = (byte*)pMem;
    for (uint32 i = 0; i < size; i++)
    {
        fputc(' ', fp);
        if (*pMemBytes >= 32 && *pMemBytes <= 127)
            fputc((char)*pMemBytes, fp);
        else
            fputc(' ', fp);
        pMemBytes++;
        fputc(' ', fp);
    }

    if (dots)
        fprintf(fp, "...");
}

inline void DumpByte(byte b, FILE* fp)
{
    char c1, c2;

    c1 = b >> 4;
    c2 = b & 0xF;

    if (c1 >= 10)
        c1 = c1 - 10 + 'A';
    else
        c1 += '0';
    if (c2 >= 10)
        c2 = c2 - 10 + 'A';
    else
        c2 += '0';

    fputc(c1, fp);
    fputc(c2, fp);
}

static void DumpBytes(const void* pMem, uint32 size, FILE* fp)
{
    bool dots;
    byte* pMemBytes;

    dots = false;
    if (size > 20)
    {
        size = 20;
        dots = true;
    }

    pMemBytes = (byte*)pMem;
    for (uint32 i = 0; i < size; i++)
    {
        DumpByte(*pMemBytes++, fp);
        fputc(' ', fp);
    }

    if (dots)
        fprintf(fp, "...");
}

static DebugMemBlockHeader* GetHeaderFromUserData(const void* userData)
{
    return reinterpret_cast<DebugMemBlockHeader*>(((byte*)userData) - sizeof(DebugMemBlockHeader));
}

//Public methods
void* MemoryTrackingAllocator::Allocate(uint64 size)
{
    this->mutex.Lock();

    byte* memoryBlock = (byte*)this->baseAllocator.Allocate(sizeof(DebugMemBlockHeader) + c_guaranteedMemoryAlignment + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    //memoryBlock is aligned, make sure that user memory is also aligned, then put header directly before that
    DebugMemBlockHeader* memBlockHeader = (DebugMemBlockHeader*)((uint8*)AlignMemoryAddress(memoryBlock + sizeof(DebugMemBlockHeader), c_guaranteedMemoryAlignment) - sizeof(DebugMemBlockHeader));

    //fill out block header
    memBlockHeader->lineNumber = currentContext.lineNumber;
    memBlockHeader->fileName = currentContext.fileName;
    memBlockHeader->next = nullptr;
    memBlockHeader->previous = this->lastMemBlock;
    memBlockHeader->seqNumber = this->seqNumber++;
    memBlockHeader->userSize = size;
    memBlockHeader->alignmentOffset = static_cast<uint32>(((byte*)memBlockHeader) - memoryBlock);

    //link block
    if (this->lastMemBlock)
        this->lastMemBlock->next = memBlockHeader;
    else
        this->firstMemBlock = memBlockHeader;
    this->lastMemBlock = memBlockHeader;

    //fill out the memory corruption detection sections
    MemSet(memBlockHeader->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, sizeof(memBlockHeader->preHeapCorriptionDetectionSection));
    MemSet(memBlockHeader->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    //init memory with something else than 0
    MemSet(memBlockHeader->GetUserData(), HEAP_INIT_VALUE, size);

    void* userData = memBlockHeader->GetUserData();

    this->mutex.Unlock();

    return userData;
}

void MemoryTrackingAllocator::CheckHeapIntegrity()
{
    this->mutex.Lock();

    for (DebugMemBlockHeader* block = this->firstMemBlock; block; block = block->next)
    {
        block->VerifyIntegrity(this->mutex);
    }

    this->mutex.Unlock();
}

_stdxx_::DebugMemBlockHeader* MemoryTrackingAllocator::DumpMemoryLeaks()
{
    if (this->firstMemBlock == nullptr)
        return nullptr;

    this->mutex.Lock(); //VERY IMPORTANT! From now on, no allocation whatsoever is allowed to be made using Std++::MemAllocDebug or the other Std++ Memory Debug functions

    FILE* fp;
    //fp = fopen("StdXX MemLeaks.txt", "w"); //we use the standard library here in order to not generate any memory allocations
    fp = stderr;

    fprintf(fp, "!!!YOU HAVE MEMORY LEAKS!!!\r\n");
    fprintf(fp, "Memory blocks that leak:\r\n");
    DebugMemBlockHeader* pBlock = this->firstMemBlock;
    while (pBlock)
    {
        fprintf(fp, "Block %d, Userdata: %p, Size: %d bytes, Allocated in: %s:%d\r\n", pBlock->seqNumber, pBlock->GetUserData(), pBlock->userSize, pBlock->fileName, pBlock->lineNumber);

        if (pBlock->IsCorrupt())
        {
            fprintf(fp, "HEAP CORRUPTED. Memory before or/and after block was modified. Value should be: ");
            DumpByte(HEAP_CORRUPTION_DETECTIONSECTION_VALUE, fp);
            fprintf(fp, "\r\n");
        }

        fprintf(fp, "Dump: \r\n");

        //heap corruption before data
        if (pBlock->IsCorrupt())
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
        if (pBlock->IsCorrupt())
        {
            fprintf(fp, "\tAfter block:\t");
            DumpBytes(pBlock->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_SIZE, fp);
            fprintf(fp, "\r\n");
        }

        pBlock = pBlock->next;
    }

    //fclose(fp);

    this->mutex.Unlock();

    return this->firstMemBlock;
}

void MemoryTrackingAllocator::Free(void* userData)
{
    if (userData == nullptr)
        return;

    this->mutex.Lock();

    DebugMemBlockHeader* memBlockHeader = GetHeaderFromUserData(userData);
    memBlockHeader->VerifyIntegrity(this->mutex);

    //remove from list
    if (memBlockHeader->next)
    {
        memBlockHeader->next->previous = memBlockHeader->previous;
    }
    else
    {
        ASSERT(this->lastMemBlock == memBlockHeader, u8"If you see this, report to StdXX");
        this->lastMemBlock = memBlockHeader->previous;
    }

    if (memBlockHeader->previous)
    {
        memBlockHeader->previous->next = memBlockHeader->next;
    }
    else
    {
        ASSERT(this->firstMemBlock == memBlockHeader, u8"If you see this, report to StdXX");
        this->firstMemBlock = memBlockHeader->next;
    }

    void* memBlock = memBlockHeader->GetAllocatedMemoryAddress();

    //clear whole memory with a random value (but not 0)
    //this option invalidates memBlockHeader!!!
    MemSet(memBlockHeader, 0xDD, sizeof(DebugMemBlockHeader) + memBlockHeader->userSize + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    this->baseAllocator.Free(memBlock);

    this->mutex.Unlock();
}

void* MemoryTrackingAllocator::Reallocate(void* mem, uint64 size)
{
    if (!mem)
        return this->Allocate(size);

    this->mutex.Lock();

    //fetch block memory header
    DebugMemBlockHeader* oldMemHeader = GetHeaderFromUserData(mem);
    oldMemHeader->VerifyIntegrity(this->mutex);

    //do reallocate
    byte* memoryBlock = (byte*)this->baseAllocator.Reallocate(oldMemHeader->GetAllocatedMemoryAddress(), sizeof(DebugMemBlockHeader) + c_guaranteedMemoryAlignment + size + HEAP_CORRUPTION_DETECTIONSECTION_SIZE);
    //memoryBlock is aligned, make sure that user memory is also aligned, then put header directly before that
    DebugMemBlockHeader* newBlock = (DebugMemBlockHeader*)((uint8*)AlignMemoryAddress(memoryBlock + sizeof(DebugMemBlockHeader), c_guaranteedMemoryAlignment) - sizeof(DebugMemBlockHeader));
    ASSERT(newBlock, u8"If you see this, report to StdXX");

    if (size > newBlock->userSize)
    {
        //fill new allocated memory with random value
        MemSet(newBlock->GetUserData() + newBlock->userSize, HEAP_INIT_VALUE, size - newBlock->userSize);
    }

    //update header
    newBlock->fileName = currentContext.fileName;
    newBlock->lineNumber = currentContext.lineNumber;
    newBlock->seqNumber = this->seqNumber++;
    newBlock->userSize = size;
    newBlock->alignmentOffset = static_cast<uint32>(((byte*)newBlock) - memoryBlock);

    //fill out the memory corruption detection section after user memory
    MemSet(newBlock->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE);

    if (oldMemHeader == newBlock)
    {
        //nothing to be done
        this->mutex.Unlock();
        return newBlock->GetUserData();
    }

    //memory has been moved...
    //new block header pointers are still correct...
    //but old block is still referenced
    if (newBlock->next)
    {
        newBlock->next->previous = newBlock->previous;
    }
    else
    {
        ASSERT(this->lastMemBlock == oldMemHeader, u8"If you see this, report to StdXX");
        this->lastMemBlock = newBlock->previous;
    }

    if (newBlock->previous)
    {
        newBlock->previous->next = newBlock->next;
    }
    else
    {
        ASSERT(this->firstMemBlock == oldMemHeader, u8"If you see this, report to StdXX");
        this->firstMemBlock = newBlock->next;
    }

    //put new block to the end of the list
    if (this->lastMemBlock)
        this->lastMemBlock->next = newBlock;
    else
        this->firstMemBlock = newBlock;

    newBlock->previous = this->lastMemBlock;
    newBlock->next = NULL;

    this->lastMemBlock = newBlock;

    void* userData = newBlock->GetUserData();
    this->mutex.Unlock();

    return userData;
}

void MemoryTrackingAllocator::SetContext(const char* fileName, uint32 lineNumber)
{
	currentContext.fileName = fileName;
	currentContext.lineNumber = lineNumber;
}