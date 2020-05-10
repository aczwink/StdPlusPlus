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
//Class header
#include <Std++/Multimedia/Demuxer.hpp>
//Local
#include <Std++/Multimedia/Decoder.hpp>
#include <Std++/Multimedia/Format.hpp>
#include <Std++/Multimedia/Parser.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

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
		if(this->streams[i]->duration == Unsigned<uint64>::Max())
			return false;
	}

	return true;
}

void Demuxer::DeriveDurationFromPacketTimestamps()
{
	uint32 i;
	uint64 currentOffset;
	Stream *pStream;
	BinaryTreeSet<uint32> streamsThatRequireDuration;

	//TODO: do this only if byte seeking is allowed

	const uint32 maxSearchSize = this->GetNumberOfStreams() * 10000;

	currentOffset = this->inputStream.GetCurrentOffset();
	this->inputStream.SeekTo(this->inputStream.QuerySize() - maxSearchSize);

	for(i = 0; i < this->streams.GetNumberOfElements(); i++)
	{
		if(this->streams[i]->duration == Unsigned<uint64>::Max())
			streamsThatRequireDuration.Insert(i);
	}

	while(true)
	{
		//work directly on packets to avoid parsing overhead
		UniquePointer<IPacket> packet = this->ReadPacket();
		if(packet.IsNull())
			break;

		if(packet->GetPresentationTimestamp() != Unsigned<uint64>::Max() && streamsThatRequireDuration.Contains(packet->GetStreamIndex()))
		{
			pStream = this->GetStream(packet->GetStreamIndex());

			pStream->duration = packet->GetPresentationTimestamp() - pStream->startTime;
		}
	}

	this->inputStream.SeekTo(currentOffset);
}

void Demuxer::ExtractInfo(const IPacket& packet)
{
	Stream *pStream;

	pStream = this->GetStream(packet.GetStreamIndex());

	//in case we have a pts and stream has no start time, we use it as start time
	if(packet.GetPresentationTimestamp() != Unsigned<uint64>::Max() && pStream->startTime == Unsigned<uint64>::Max())
	{
		pStream->startTime = packet.GetPresentationTimestamp();
	}

	//check for coding format
	if (pStream->GetCodingFormat())
	{		
		DecoderContext *decoderContext = pStream->GetDecoderContext();
		if (decoderContext == nullptr)
		{
			//try to allocate one
			const Decoder *decoder = pStream->GetCodingFormat()->GetBestMatchingDecoder();
			if (decoder)
			{
				decoderContext = decoder->CreateContext(*pStream);
				pStream->SetDecoderContext(decoderContext);
			}
			else
			{
				//shit.. we don't have a decoder...

				//if we have a parser... it may suggest a codec id
				if (ParserContext *parserContext = pStream->GetParserContext())
				{
					NOT_IMPLEMENTED_ERROR; //TODO:; next lines
					/*
					CodecId codecId;
					
					codecId = pParser->GetCodecId();
					pStream->SetCodec(Codec::GetCodec(codecId));
					pDecoder = pStream->GetDecoder();
					
					if(pDecoder)
					{
					//juhu we got a decoder now
					pDecoder->Decode(refPacket);
					}
					*/
				}
			}
		}

		if (decoderContext != nullptr)
		{
			//we have a decoder.... let's try if decoding some packets updates info
			decoderContext->Decode(packet);
		}
	}
}

//Public methods
bool Demuxer::FindStreamInfo()
{
	bool ret;
	uint16 nReadFrames;
	uint64 currentOffset;
	FormatInfo formatInfo;

	const uint16 MAX_PROBE_FRAMES = this->GetNumberOfStreams() * 10;

	ret = true;

	this->refFormat.GetFormatInfo(formatInfo);
	currentOffset = this->inputStream.GetCurrentOffset();

	//initialize parsers
	for (Stream *& stream : this->streams)
	{
		if ((stream->GetCodingFormat() != nullptr) && (stream->parserFlags.requiresParsing))
		{
			ParserContext *parserContext = stream->GetParserContext();
			if (parserContext == nullptr)
			{
				//try to allocate one
				const Parser *parser = stream->GetCodingFormat()->GetBestMatchingParser();
				if (parser)
				{
					parserContext = parser->CreateContext(*stream);
					stream->SetParserContext(parserContext);
				}
			}
		}
	}

	//read some frames to get stream info
	nReadFrames = 0;
	while(!this->AllInfoIsAvailable())
	{
		if(nReadFrames >= MAX_PROBE_FRAMES)
			break;

		UniquePointer<IPacket> packet = this->ReadFrame();
		if(packet.IsNull())
			break;
		this->ExtractInfo(*packet);

		nReadFrames++;
	}

	if(formatInfo.supportsByteSeeking && !this->AllStreamsHaveDuration())
		this->DeriveDurationFromPacketTimestamps();

	//derive overall timing info
	this->UpdateTimingInfo();

	//compute overall bit rate
	if(this->AllStreamsHaveDuration())
		this->bitRate = uint32(((float64) this->inputStream.QuerySize() * 8 / this->timeScale) / this->duration);

	//reset state
	this->inputStream.SeekTo(currentOffset);
	this->Reset();
	for(uint32 i = 0; i < this->streams.GetNumberOfElements(); i++)
	{
		if(this->streams[i]->GetParserContext())
			this->streams[i]->GetParserContext()->Reset();
		if(this->streams[i]->GetDecoderContext())
			this->streams[i]->GetDecoderContext()->Reset();
	}

	return this->AllInfoIsAvailable();
}

UniquePointer<IPacket> Demuxer::ReadFrame()
{
	uint32 i;

	//Check if we have a queued package ready
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		if(ParserContext *parserContext = this->GetStream(i)->GetParserContext())
		{
			if(parserContext->IsFrameReady())
				return new Packet(parserContext->GetParsedFrame());
		}
	}

	//read packets until a frame is collected
	while(true)
	{
		UniquePointer<IPacket> packet = this->ReadPacket();
		if(packet.IsNull())
			break;

		if(ParserContext *parserContext = this->GetStream(packet->GetStreamIndex())->GetParserContext())
		{
			parserContext->Parse(*packet);
			if (parserContext->ShouldRepack())
			{
				if (parserContext->IsFrameReady())
					return new Packet(parserContext->GetParsedFrame());
			}
			else
			{
				//the parser should parse the packet but not repack it. Therefore we return it
				break;
			}
		}
		else
		{
			//no parser... well then just return packet
			break;
		}
	}

	return nullptr;
}

//Protected methods
void Demuxer::Reset()
{
}