/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
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
	Math::Rational<uint64> timeCodeScale;
	struct
	{
		bool isClusterOpen;
		uint64 beginOffset;
		uint32 size;
		uint64 pts;
		uint64 basePTS;
	} currentCluster;
	BinaryTreeMap<uint64, CueEntry> cues;

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
	void BeginElement(MatroskaId id);
	void ComputeTimeScales();
	void EndElement();
	void FinalizeMetaSeekInfo();
	uint64 PrepareMetaSeekEntry(MatroskaId id);
	void PrepareMetaSeekInfo();
	void WriteAdditionalAudioStreamInfo(AudioStream &refStream);
	void WriteCodecElement(Stream &stream);
	void WriteCuePoints();
	void WriteEBMLUInt(uint64 value);
	void WriteId(MatroskaId id);
	void WriteUInt(uint64 value);

	//Inline
	inline uint64 MapSeconds(uint64 seconds) const
	{
		return this->timeScale.Rescale(seconds);
	}

	inline uint64 TransformPTS(uint64 pts, uint32 streamIndex) const
	{
		if(pts == Unsigned<uint64>::Max())
			return Unsigned<uint64>::Max();

		return this->GetStream(streamIndex)->timeScale.Rescale(pts, this->timeScale);
	}

	inline void WriteASCIIElement(MatroskaId id, const String &string)
	{
		this->BeginElement(id);
		TextWriter writer(this->outputStream, TextCodecType::ASCII);
		writer.WriteString(string);
		this->EndElement();
	}

	inline void WriteFloatElement(MatroskaId id, float64 value)
	{
		this->BeginElement(id);
		DataWriter dataWriter(true, this->outputStream);
		dataWriter.WriteFloat64(value);
		this->EndElement();
	}

	inline void WriteUIntElement(MatroskaId id, uint64 value)
	{
		this->BeginElement(id);
		this->WriteUInt(value);
		this->EndElement();
	}

	inline void WriteUTF8Element(MatroskaId id, const String &string)
	{
		this->BeginElement(id);
		TextWriter textWriter(this->outputStream, TextCodecType::UTF8);
		textWriter.WriteString(string);
		this->EndElement();
	}

public:
	//Constructor
	MatroskaMuxer(const Format &refFormat, SeekableOutputStream &refOutput);

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const IPacket& packet);
};