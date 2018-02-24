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
#include "WAVE_Muxer.hpp"
//Local
#include <Std++/Multimedia/AudioStream.hpp>
#include "WAVE.h"

//Constructor
WAVE_Muxer::WAVE_Muxer(const Format &refFormat, ASeekableOutputStream &refOutput) : Muxer(refFormat, refOutput)
{
}

//Private methods
uint16 WAVE_Muxer::GetBitsPerSample(CodecId codecId) const
{
	switch(codecId)
	{
		case CodecId::PCM_Float32LE:
			return 32;
		case CodecId::PCM_S16LE:
			return 16;
	}

	NOT_IMPLEMENTED_ERROR;
	return 0;
}

//Public methods
void WAVE_Muxer::Finalize()
{
	uint32 currentOffset;

	currentOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.SetCurrentOffset(this->riffTagSizeOffset);
	this->refOutput.WriteUInt32LE(currentOffset - 8); //sizeof("RIFF") + sizeof(chunkSize)

	this->refOutput.SetCurrentOffset(this->dataChunkSizeOffset);
	this->refOutput.WriteUInt32LE(currentOffset - this->dataChunkSizeOffset - 4);
}

void WAVE_Muxer::WriteHeader()
{
	uint16 bitsPerSample, blockAlign;

	AudioStream *stream = (AudioStream *)this->GetStream(0);

	bitsPerSample = this->GetBitsPerSample(stream->GetCodec()->GetId());
	blockAlign = stream->nChannels * bitsPerSample / 8;

	//riff chunk
	this->refOutput.WriteUInt32LE(WAVE_RIFFCHUNK_CHUNKID);
	this->riffTagSizeOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.WriteUInt32LE(0); //file size
	this->refOutput.WriteUInt32LE(WAVE_RIFFCHUNK_RIFFTYPE);

	//format chunk
	this->refOutput.WriteUInt32LE(WAVE_FORMATCHUNK_CHUNKID);
	this->refOutput.WriteUInt32LE(16); //size of format chunk
	this->refOutput.WriteUInt16LE(MapToTwoCC(stream->GetCodec()->GetId()));
	this->refOutput.WriteUInt16LE(stream->nChannels);
	this->refOutput.WriteUInt32LE(stream->sampleRate);
	this->refOutput.WriteUInt32LE(stream->sampleRate * blockAlign);
	this->refOutput.WriteUInt16LE(blockAlign);
	this->refOutput.WriteUInt16LE(bitsPerSample);

	//data chunk
	this->refOutput.WriteUInt32LE(WAVE_DATACHUNK_CHUNKID);
	this->dataChunkSizeOffset = (uint32)this->refOutput.GetCurrentOffset();
	this->refOutput.WriteUInt32LE(0); //chunk size
}

void WAVE_Muxer::WritePacket(const Packet &packet)
{
	this->refOutput.WriteBytes(packet.GetData(), packet.GetSize());
}