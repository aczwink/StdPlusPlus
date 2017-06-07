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
#include <ACStdLib/Multimedia/Demuxer.hpp>
//Local
#include <ACStdLib/Multimedia/Format.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Private methods
bool Demuxer::AllInfoIsAvailable()
{
	for(uint8 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		if(!this->GetStream(i)->AllInfoIsAvailable())
			return false;
	}

	return true;
}

bool Demuxer::AllStreamsHaveDuration() const
{
	for(uint8 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		if(this->streams[i]->duration == UINT64_MAX)
			return false;
	}

	return true;
}

void Demuxer::DeriveDurationFromPacketTimestamps()
{
	uint32 i;
	uint64 currentOffset;
	Packet packet;
	Stream *pStream;
	FiniteSet<uint32> streamsThatRequireDuration;

	//TODO: do this only if byte seeking is allowed

	const uint32 maxSearchSize = this->GetNumberOfStreams() * 10000;

	currentOffset = this->inputStream.GetCurrentOffset();
	this->inputStream.SetCurrentOffset(this->inputStream.GetSize() - maxSearchSize);

	for(i = 0; i < this->streams.GetNumberOfElements(); i++)
	{
		if(this->streams[i]->duration == UINT64_MAX)
			streamsThatRequireDuration.Insert(i);
	}

	while(true)
	{
		//work directly on packets to avoid parsing overhead
		if(!this->ReadPacket(packet))
			break;

		if(packet.pts != UINT64_MAX && streamsThatRequireDuration.Contains(packet.streamIndex))
		{
			pStream = this->GetStream(packet.streamIndex);

			pStream->duration = packet.pts - pStream->startTime;
		}
	}

	this->inputStream.SetCurrentOffset(currentOffset);
}

void Demuxer::ExtractInfo(Packet &refPacket)
{
	Decoder *pDecoder;
	AParser *pParser;
	Stream *pStream;

	pStream = this->GetStream(refPacket.streamIndex);
	pDecoder = pStream->GetDecoder();
	pParser = pStream->GetParser();

	//in case we have a pts and stream has no start time, we use it as start time
	if(refPacket.pts != UINT64_MAX && pStream->startTime == UINT64_MAX)
	{
		pStream->startTime = refPacket.pts;
	}

	if(pDecoder)
	{
		//we have a decoder.... let's try if decoding some packets updates info

		pDecoder->Decode(refPacket);
	}
	else
	{
		//shit.. we don't have a decoder...

		//if we have a parser... it may suggest a codec id
		if(pParser)
		{
			CodecId codecId;

			codecId = pParser->GetCodecId();
			pStream->SetCodec(Codec::GetCodec(codecId));
			pDecoder = pStream->GetDecoder();

			if(pDecoder)
			{
				//juhu we got a decoder now
				pDecoder->Decode(refPacket);
			}
		}
	}
}

//Public methods
bool Demuxer::FindStreamInfo()
{
	bool ret;
	uint16 nReadFrames;
	uint64 currentOffset;
	Packet packet;
	FormatInfo formatInfo;

	const uint16 MAX_PROBE_FRAMES = this->GetNumberOfStreams() * 10;

	ret = true;

	this->refFormat.GetFormatInfo(formatInfo);
	currentOffset = this->inputStream.GetCurrentOffset();

	//read some frames to get stream info
	nReadFrames = 0;
	while(!this->AllInfoIsAvailable())
	{
		if(nReadFrames >= MAX_PROBE_FRAMES)
			break;

		if(!this->ReadFrame(packet))
			break;

		this->ExtractInfo(packet);

		nReadFrames++;
	}

	if(formatInfo.supportsByteSeeking && !this->AllStreamsHaveDuration())
		this->DeriveDurationFromPacketTimestamps();

	//derive overall timing info
	this->UpdateTimingInfo();

	//compute overall bit rate
	if(this->AllStreamsHaveDuration())
		this->bitRate = uint32(((float64)this->inputStream.GetSize() * 8 / this->timeScale) / this->duration);

	//reset state
	this->inputStream.SetCurrentOffset(currentOffset);
	for(uint32 i = 0; i < this->streams.GetNumberOfElements(); i++)
	{
		if(this->streams[i]->GetParser())
			this->streams[i]->GetParser()->Reset();
		if(this->streams[i]->GetDecoder())
			this->streams[i]->GetDecoder()->Reset();
	}

	return this->AllInfoIsAvailable();
}

bool Demuxer::ReadFrame(Packet &packet)
{
	uint32 i;
	AParser *pParser;

	//Check if we have a queued package ready
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		if(pParser = this->GetStream(i)->GetParser())
		{
			if(pParser->IsFrameReady())
			{
				pParser->GetParsedFrame(packet);

				return true;
			}
		}
	}

	//read packets until a frame is collected
	while(true)
	{
		if(!this->ReadPacket(packet))
			return false;

		if(pParser = this->GetStream(packet.streamIndex)->GetParser())
		{
			pParser->Parse(packet);

			if(pParser->IsFrameReady())
			{
				pParser->GetParsedFrame(packet);
				break;
			}
		}
		else
		{
			//no parser... well then just return packet
			break;
		}
	}

	return true;
}