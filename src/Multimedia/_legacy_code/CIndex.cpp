//Class Header
#include "__CIndex.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Functions
void CIndex::Add(uint32 streamIndex, uint32 offset, uint32 size, uint64 timestamp, bool isKeyFrame)
{
	SIndexEntry ie;
	int32 index;
	CArray<SIndexEntry> &refStreamIndices = this->indices[streamIndex];

	ie.offset = offset;
	ie.size = size;
	ie.timestamp = timestamp;
	ie.isKeyFrame = isKeyFrame;
	
	index = this->Find(streamIndex, timestamp);
	
	if(index == -1)
	{
		refStreamIndices.Push(ie);
	}
}

int32 CIndex::Find(uint32 streamIndex, uint64 timestamp, bool backwards) const
{
	int32 a, b, m;
	uint64 ts;
	const CArray<SIndexEntry> &refStreamIndices = this->indices[streamIndex];

	a = -1;
	b = refStreamIndices.GetNoOfElements();

	if(b && refStreamIndices[b-1].timestamp < timestamp)
		a = b - 1;

	while(b - a > 1)
	{
		m = (a + b) >> 1;
		ts = refStreamIndices[m].timestamp;
		if(ts >= timestamp)
			b = m;
		if(ts <= timestamp)
			a = m;
	}
	m = backwards ? a : b;

	if(m == refStreamIndices.GetNoOfElements())
		return -1;
	return m;
}

const CArray<SIndexEntry> &CIndex::GetStreamIndices(uint32 streamIndex) const
{
	return this->indices[streamIndex];
}

void CIndex::Release()
{
	this->indices.Release();
}