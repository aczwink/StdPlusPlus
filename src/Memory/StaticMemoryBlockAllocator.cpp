/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Memory/StaticMemoryBlockAllocator.hpp>
//Namespaces
using namespace StdXX::Memory;

struct StaticMemoryBlockAllocator::Block
{
    uint64 payloadSize;
    bool isFree;

    //Properties
    inline Block* Next() const
    {
        return reinterpret_cast<Block *>(this->UserData() + this->payloadSize);
    }

    inline uint64 Size() const
    {
        return this->payloadSize + sizeof(Block);
    }

    inline uint8* UserData() const
    {
        return (uint8 *) (this + 1);
    }
};

//Constructor
StaticMemoryBlockAllocator::StaticMemoryBlockAllocator(uint64 size)
{
    this->dataBlock = (uint8*)MemAlloc(size);
    this->size = size;

    this->tail = this->Head();
    this->tail->payloadSize = size - sizeof(Block);
    this->tail->isFree = true;
}

//Public methods
void* StaticMemoryBlockAllocator::Allocate(uint64 size)
{
    if(this->tail->isFree and (size <= this->tail->payloadSize))
    {
        Block* current = this->tail;

        this->SplitBlock(*this->tail, size);
        this->tail = this->tail->Next();

        current->isFree = false;

        return current->UserData();
    }

    for(Block* current = this->Head(); current < this->tail; current = current->Next())
    {
        if(current->isFree and (size <= current->payloadSize))
        {
            this->SplitBlock(*current, size);
            current->isFree = false;
            return current->UserData();
        }
    }

    return nullptr;
}

void StaticMemoryBlockAllocator::Free(void* mem)
{
    Block* block = reinterpret_cast<Block *>(static_cast<uint8 *>(mem) - sizeof(Block));
    block->isFree = true;

    Block* next = block->Next();
    if(next->isFree)
        block->payloadSize += next->payloadSize + sizeof(Block);
}

void *StaticMemoryBlockAllocator::Reallocate(void* mem, uint64 size)
{
    if(mem == nullptr)
        return this->Allocate(size);

    Block* block = reinterpret_cast<Block *>(static_cast<uint8 *>(mem) - sizeof(Block));
    if(size <= block->payloadSize)
        return mem;

    Block* nextBlock = block->Next();
    if( nextBlock->isFree and (size <= (block->payloadSize + nextBlock->Size())) )
    {
        block->payloadSize += nextBlock->Size();
        this->SplitBlock(*block, size);

        block->Next()->isFree = true;
        if(this->tail == nextBlock)
            this->tail = block->Next();
        return mem;
    }

    void* newMem = this->Allocate(size);
    MemCopy(newMem, mem, block->payloadSize);
    this->Free(mem);
    return newMem;
}

//Private methods
void StaticMemoryBlockAllocator::SplitBlock(Block& block, uint64 newSize) const
{
    uint64 newSizeWithBlockHeader = newSize + sizeof(Block);
    if(newSizeWithBlockHeader > block.payloadSize)
        return;

    uint64 nextBlockSize = block.payloadSize - newSizeWithBlockHeader;
    block.payloadSize = newSize;

    Block* next = block.Next();
    next->payloadSize = nextBlockSize;
    next->isFree = block.isFree;
}