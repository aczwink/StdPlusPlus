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
//Definitions
#define HEAP_CORRUPTION_DETECTIONSECTION_SIZE 12
#define HEAP_CORRUPTION_DETECTIONSECTION_VALUE 0xFD

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
struct _stdxx_::DebugMemBlockHeader
{
	DebugMemBlockHeader* previous;
	DebugMemBlockHeader* next;
	const char* fileName;
	uint32 lineNumber;
	uint64 userSize;
	uint32 seqNumber;
	uint32 alignmentOffset;
	byte preHeapCorriptionDetectionSection[HEAP_CORRUPTION_DETECTIONSECTION_SIZE];

	//Inline
	inline void* GetAllocatedMemoryAddress() const
	{
		return ((byte*)this) - this->alignmentOffset;
	}

	inline byte* GetPostHeapCorruptionDetectionAddress() const
	{
		//the region is right after the user data
		return this->GetUserData() + this->userSize;
	}

	inline byte* GetUserData() const
	{
		//after the header is the user data
		return (byte*)(this + 1);
	}

	inline bool IsCorrupt() const
	{
		//check heap corruption detection section before user memory
		if (!this->CheckBytes(this->preHeapCorriptionDetectionSection, HEAP_CORRUPTION_DETECTIONSECTION_VALUE, sizeof(this->preHeapCorriptionDetectionSection)))
			return true;
		//check heap corruption detection section after user memory
		if (!this->CheckBytes(this->GetPostHeapCorruptionDetectionAddress(), HEAP_CORRUPTION_DETECTIONSECTION_VALUE, HEAP_CORRUPTION_DETECTIONSECTION_SIZE))
			return true;

		return false;
	}

	inline void VerifyIntegrity(StdXX::Mutex& mutex) const
	{
		if (this->IsCorrupt())
		{
			mutex.Unlock(); //we need to free the lock so that ASSERT can allocate
			ASSERT(false, u8"HEAP CORRUPTED. Check memory dump!");
			mutex.Lock();
		}
	}

private:
	//Inline
	inline bool CheckBytes(const void* bytes, byte mustBeValue, uint32 size) const
	{
		byte* ptr = (byte*)bytes;
		while (size--)
			if (*ptr++ != mustBeValue)
				return false;

		return true;
	}
};