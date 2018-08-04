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
#include <Std++/Multimedia/Muxer.hpp>
#include <Std++/Multimedia/AudioStream.hpp>
#include "Matroska.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;
using namespace Matroska;

class MatroskaMuxer : public Muxer
{
	struct CueEntry
	{
		uint64 clusterOffset;
		uint32 relativeOffset;
		DynamicArray<uint32> streamIndices;
	};
private:
	//Members
	DynamicArray<uint64> elementSizeOffsets;
	Fraction timeCodeScale;
	struct
	{
		bool isClusterOpen;
		uint64 beginOffset;
		uint32 size;
		uint64 pts;
		uint64 basePTS;
	} currentCluster;
	Map<uint64, CueEntry> cues;

	//meta seeking
	uint64 segmentOutputStreamOffset;
	struct
	{
		uint64 segmentInfoOffset;
		uint64 trackInfoOffset;
		uint64 cueingDataOffset;
	} metaSeekInfoOffsets;
	struct
	{
		uint64 segmentInfoOffset;
		uint64 trackInfoOffset;
		uint64 cueingDataOffset;
	} metaSeekInfoWriteOffsets;

	//Methods
	void BeginElement(EMatroskaId id);
	void ComputeTimeScales();
	void EndElement();
	void FinalizeMetaSeekInfo();
	uint64 PrepareMetaSeekEntry(EMatroskaId id);
	void PrepareMetaSeekInfo();
	void WriteAdditionalAudioStreamInfo(AudioStream &refStream);
	void WriteCodecElement(Stream &refStream);
	void WriteCuePoints();
	void WriteEBMLUInt(uint64 value);
	void WriteId(EMatroskaId id);
	void WriteUInt(uint64 value);

	//Inline
	inline uint64 MapSeconds(uint64 seconds) const
	{
		return seconds / this->timeScale;
	}

	inline uint64 TransformPTS(uint64 pts, uint32 streamIndex) const
	{
		if(pts == Natural<uint64>::Max())
			return Natural<uint64>::Max();

		return pts / this->timeScale * this->GetStream(streamIndex)->timeScale;
	}

	inline void WriteASCIIElement(EMatroskaId id, const ByteString &refString)
	{
		this->BeginElement(id);
		this->outputStream.WriteBytes(refString.GetC_Str(), refString.GetLength());
		this->EndElement();
	}

	inline void WriteFloatElement(EMatroskaId id, float64 value)
	{
		this->BeginElement(id);
		this->outputStream.WriteFloat64BE(value);
		this->EndElement();
	}

	inline void WriteUIntElement(EMatroskaId id, uint64 value)
	{
		this->BeginElement(id);
		this->WriteUInt(value);
		this->EndElement();
	}

	inline void WriteUTF8Element(EMatroskaId id, const UTF8String &refString)
	{
		this->BeginElement(id);
		this->outputStream.WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());
		this->EndElement();
	}

public:
	//Constructor
	MatroskaMuxer(const Format &refFormat, ASeekableOutputStream &refOutput);

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const Packet &packet);
};