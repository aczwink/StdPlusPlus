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
#pragma once
//Local
#include <Std++/Memory.hpp>
#include "Allocator.hpp"

namespace StdXX::Memory
{
    class StaticMemoryBlockAllocator : public Allocator
    {
        struct Block;
    public:
        //Constructor
        StaticMemoryBlockAllocator(uint64 size);

        //Destructor
        inline ~StaticMemoryBlockAllocator()
        {
            MemFree(this->dataBlock);
        }

        void* Allocate(uint64 size) override;
        void Free(void *mem) override;
        void* Reallocate(void *mem, uint64 size) override;

    private:
        //Members
        uint8* dataBlock;
        uint64 size;
        Block* tail;

        //Properties
        inline uint8* EndOfDataBlock() const
        {
            return this->dataBlock + this->size;
        }

        inline Block* Head()
        {
            return reinterpret_cast<Block *>(this->dataBlock);
        }

        //Methods
        void SplitBlock(Block& block, uint64 newSize) const;
    };
}