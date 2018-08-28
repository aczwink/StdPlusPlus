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
#include <Std++/Multimedia/BlockIndex.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
bool BlockIndex::FindEntry(uint64 offset, uint32 &entryIndex)
{
	this->EnsureSorted();

	uint32 low = 0;
	uint32 high = this->GetNumberOfEntries() - 1;

	while(low <= high)
	{
		entryIndex = low + (high - low) / 2;

		if(this->GetEntry(entryIndex).Contains(offset))
			return true;

		if(this->GetEntry(entryIndex).offset > offset)
			high = entryIndex - 1;
		else
			low = entryIndex + 1;
	}

	return false;
}

uint64 BlockIndex::GetStartOffset()
{
	this->EnsureSorted();

	if(this->GetEntries().IsEmpty())
		return 0;

	return this->GetEntry(0).offset;
}