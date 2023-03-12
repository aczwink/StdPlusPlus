/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include "WAVE_Demuxer.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "WAVE.hpp"
#include "../../ID3.hpp"
//Definitions
#define WAVE_LISTCHUNK_CHUNKID 0x5453494C
#define WAVE_MAXPACKETSIZE 4096

//Public methods
void WAVE_Demuxer::ReadHeader()
{
	DataReader dataReader(false, this->inputStream);

	dataReader.Skip(4); //"RIFF"
	dataReader.Skip(4); //chunkSize - should be fileSize
	dataReader.Skip(4); //"WAVE"

	uint16 bitsPerSample;
	uint32 chunkId, chunkSize;
	while(!this->inputStream.IsAtEnd())
	{
		chunkId = dataReader.ReadUInt32();
		chunkSize = dataReader.ReadUInt32();

		switch(chunkId)
		{
			case WAVE_FORMATCHUNK_CHUNKID: //"fmt "
				this->ReadFmtChunk(chunkSize, bitsPerSample);
				break;
			case WAVE_DATACHUNK_CHUNKID: //"data"
				this->dataOffset = this->inputStream.QueryCurrentOffset();
				this->dataSize = chunkSize;
				this->inputStream.Skip(chunkSize);
				break;
			case WAVE_LISTCHUNK_CHUNKID: //skip
				this->inputStream.Skip(chunkSize);
				break;
			case WAVE_ID3CHUNK_CHUNKID: //"id3 "
				ReadID3V2Tags(this->inputStream, this->metaInfo);
				break;
			default:
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
	this->inputStream.SeekTo(this->dataOffset);

	if(bitsPerSample)
	{
		Stream *pStream = this->GetStream(0);
		uint32 nSamples = this->dataSize / (pStream->codingParameters.audio.sampleFormat->nChannels * bitsPerSample / 8);
		pStream->duration = nSamples;
	}
}

void WAVE_Demuxer::Seek(uint64 timestamp, const class TimeScale &timeScale)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Private methods
void WAVE_Demuxer::ReadFmtChunk(uint32 chunkSize, uint16 &refBitsPerSample)
{
	Stream* pStream = new Stream(DataType::Audio);
	ReadWaveHeader(this->inputStream, *pStream, chunkSize, this->blockAlign, refBitsPerSample);

	pStream->startTime = 0;
	pStream->timeScale = {1, pStream->codingParameters.audio.sampleRate};

	this->AddStream(pStream);
}

UniquePointer<IPacket> WAVE_Demuxer::ReadPacket()
{

	SeekableInputStream *pInput = &this->inputStream;
	Stream* pStream = this->GetStream(0);

	if(pInput->QueryCurrentOffset() >= this->DataEndOffset())
		return nullptr;

	uint32 left = this->DataEndOffset() - pInput->QueryCurrentOffset();
	uint16 size = WAVE_MAXPACKETSIZE;

	if(this->blockAlign > 1)
	{
		size = (size / this->blockAlign) * this->blockAlign;
	}
	size = Math::Min((uint32)size, left);

	UniquePointer<Packet> packet = new Packet;

	packet->Allocate(size);
	pInput->ReadBytes(packet->GetData(), size);

	packet->streamIndex = 0;

	return packet;
}
