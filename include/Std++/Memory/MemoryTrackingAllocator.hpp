/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include "Allocator.hpp"
#include <Std++/Multitasking/Mutex.hpp>

namespace _stdxx_
{
	//Forward declarations
	struct DebugMemBlockHeader;
}

namespace StdXX::Memory
{
	class MemoryTrackingAllocator : public Allocator
	{
	public:
		//Constructor
		inline MemoryTrackingAllocator(Allocator& baseAllocator) : baseAllocator(baseAllocator)
		{
			this->firstMemBlock = nullptr;
			this->lastMemBlock = nullptr;
			this->seqNumber = 1;
		}

		//Methods
		void* Allocate(uint32 size) override;
		void CheckHeapIntegrity();
		bool DumpMemoryLeaks();
		void Free(void* mem) override;
		void* Reallocate(void* mem, uint32 size) override;

		/**
		* Sets the context before the next allocation operation for the calling thread.
		*/
		void SetContext(const char* fileName, uint32 lineNumber);

	private:
		Allocator& baseAllocator;
		Mutex mutex;
		_stdxx_::DebugMemBlockHeader* firstMemBlock;
		_stdxx_::DebugMemBlockHeader* lastMemBlock;
		uint32 seqNumber;
	};
}