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
//Local
#include <ACStdLib/Multimedia/ClusterIndex.hpp>
#include <ACStdLib/Multimedia/Demuxer.hpp>
#include <ACStdLib/Multimedia/Format.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

/*
Sources:
https://www.matroska.org/technical/specs/index.html
*/

enum class EMatroskaType
{
	ASCII_String,
	Binary,
	Date,
	Float,
	Master,
	UInt, //1 to 8 byte
	UTF8
};

struct SElemInfo
{
	EMatroskaType type;
};

struct STrackInfo
{
	uint64 number;
	uint8 type;
	CodecId codecId;

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
private:
	//Members
	ClusterIndex clusters;
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

	//Methods
	void AddStream();
	void BeginParseChilds(uint64 id);
	uint64 DecodeVariableLengthInteger(uint8 &refLength);
	void EndParseChilds(uint64 id);
	bool GetElementInfo(uint64 id, SElemInfo &refElemInfo);
	CodecId MapCodecId(const ByteString &refString);
	void ParseASCIIString(uint64 id, const ByteString &refString);
	void ParseBinary(uint64 id, uint64 size);
	uint64 ParseElement();
	void ParseFloat(uint64 id, float64 value);
	void ParseUInt(uint64 id, uint64 value);

	//Inline
	inline uint64 PutLength(uint64 uncoded, uint8 length)
	{
		return uncoded | (1ULL << uint64((length-1) * 8 + (8 - length)));
	}

public:
	//Constructor
	MatroskaDemuxer(const Format &refFormat, SeekableInputStream &refInput);

	//Methods
	void ReadHeader();
	bool ReadPacket(Packet &packet);
};