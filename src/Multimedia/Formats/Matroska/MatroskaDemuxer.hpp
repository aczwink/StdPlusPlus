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
//Local
#include <Std++/Multimedia/BlockIndex.hpp>
#include <Std++/Multimedia/Demuxer.hpp>
#include <Std++/Multimedia/Format.hpp>
#include "Matroska.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

/*
Sources:
https://www.matroska.org/technical/specs/index.html
*/
class MatroskaDemuxer : public Demuxer
{
	struct IncomingPacket
	{
		uint64 trackNumber;
		Packet packet;
		uint64 size;

		inline bool IsBuffered() const
		{
			return this->size == 0;
		}
	};

	struct TrackInfo
	{
		uint32 streamIndex;
		DynamicArray<byte> strippedHeader;
	};
public:
	//Constructor
	MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput);

	//Methods
	void ReadHeader();
	bool ReadPacket(Packet &packet);

private:
	//Members
	_stdxx_::CodingFormatIdMap<String> codecIdMap;
	Map<uint64, TrackInfo> tracks;
	TimeIndex<Matroska::CuePoint> cues;
	struct
	{
		uint64 clusterTimecode;
		LinkedList<IncomingPacket> packetQueue;
	} demuxerState;

	//Methods
	void AddStream(Matroska::Track &track);
	void BufferPackets();
	uint8 ReadBlockHeader(bool simple, uint32 blockSize);
	void ReadSegment(uint64 segmentOffset, bool isLive);
	void ReadSection(const EBML::Element &element);
	void Reset();
	/*
	 * We currently only support resyncing on the segment element. Everything else (appart from clusters) should have no unknown size and therefore no need to resync.
	 * Currently, the stream is required to be seekable here, which is stupid of course.
	 */
	void Resync();
};