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
#include <Std++/Multimedia/Index.hpp>
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

struct STrackInfo
{
	uint64 number;
	uint8 type;
	CodingFormatId codingFormatId;

	struct
	{
		uint8 bitDepth;
		uint8 nChannels;
		float64 samplingFrequency;
	} audio;

	struct
	{
		bool isMS_BMPHeader;
		bool isIntegerPCM;
		bool isFloatPCM;
		uint64 offset;
		uint64 size;
	} codecPrivate;
};

class MatroskaDemuxer : public Demuxer
{
public:
	//Constructor
	MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput);

	//Methods
	void ReadHeader();
	bool ReadPacket(Packet &packet);

private:
	//Members
	_stdxx_::CodingFormatIdMap<String> codecIdMap;
	Index clusters;
	Map<uint64, uint32> trackToStreamMap;
	struct
	{
		struct
		{
			uint64 timeCode;
			DynamicArray<uint64> offsets;
			DynamicArray<uint64> sizes;
		} currentCluster;

		STrackInfo currentTrack;
	} parserState;
	struct
	{
		uint32 blockStreamIndex;
		LinkedList<uint64> lacedFrameSizes;
	} demuxerState;

	//Methods
	void AddStream();
};