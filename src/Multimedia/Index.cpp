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
//Class header
#include <Std++/Multimedia/Index.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
Index::Index()
{
	this->entriesSorted = true;
}

//Public methods
void Index::AddEntry(uint64 offset, uint64 size, uint64 timeStamp)
{
	IndexEntry entry;

	entry.offset = offset;
	entry.size = size;
	entry.timeStamp = timeStamp;

	if(this->entries.IsEmpty())
	{
		this->entriesSorted = true;
	}
	else
	{
		this->entriesSorted = this->entriesSorted && this->entries[this->entries.GetNumberOfElements() - 1].GetEndOffset() <= offset;
	}

	this->entries.Push(entry);
}

bool Index::FindEntry(uint64 offset, uint32 &entryIndex)
{
	uint32 low, high;

	this->EnsureSorted();

	low = 0;
	high = this->entries.GetNumberOfElements() - 1;

	while(low <= high)
	{
		entryIndex = low + (high - low) / 2;

		if(this->entries[entryIndex].Contains(offset))
			return true;

		if(this->entries[entryIndex].offset > offset)
			high = entryIndex - 1;
		else
			low = entryIndex + 1;
	}

	return false;
}

uint64 Index::GetStartOffset()
{
	this->EnsureSorted();

	if(this->entries.IsEmpty())
		return 0;

	return this->entries[0].offset;
}