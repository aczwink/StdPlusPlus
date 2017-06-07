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
//Class header
#include <ACStdLib/Multimedia/ClusterIndex.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Constructor
ClusterIndex::ClusterIndex()
{
	this->clustersSorted = true;
}

//Public methods
void ClusterIndex::AddCluster(uint64 offset, uint64 size, uint64 timeStamp)
{
	CClusterEntry entry;

	entry.offset = offset;
	entry.size = size;
	entry.timeStamp = timeStamp;

	if(this->clusters.IsEmpty())
	{
		this->clustersSorted = true;
	}
	else
	{
		this->clustersSorted = this->clustersSorted && this->clusters[this->clusters.GetNumberOfElements() - 1].GetEndOffset() <= offset;
	}

	this->clusters.Push(entry);
}

bool ClusterIndex::FindEntry(uint64 offset, uint32 &refClusterIndex)
{
	uint32 low, high;

	this->EnsureSorted();

	low = 0;
	high = this->clusters.GetNumberOfElements() - 1;

	while(low <= high)
	{
		refClusterIndex = low + (high - low) / 2;

		if(this->clusters[refClusterIndex].Contains(offset))
			return true;

		if(this->clusters[refClusterIndex].offset > offset)
			high = refClusterIndex - 1;
		else
			low = refClusterIndex + 1;
	}

	return false;
}

uint64 ClusterIndex::GetStartOffset()
{
	this->EnsureSorted();

	if(this->clusters.IsEmpty())
		return 0;

	return this->clusters[0].offset;
}