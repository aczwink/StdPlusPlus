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
#include <Std++/Multimedia/MediaPlayer.hpp>
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;

//Constructor
DecoderThread::DecoderThread(StdPlusPlus::Multimedia::MediaPlayer *player, CodecId encodingCodec)
{
	this->player = player;
	this->shutdown = false;
	this->work = false;
	this->working = false;
	this->decoder = nullptr;
	this->streamIndex = Natural<uint32>::Max();
	this->encoder = Codec::GetCodec(encodingCodec)->CreateEncoder();
}

//Destructor
DecoderThread::~DecoderThread()
{
	this->FlushInputQueue();
	this->FlushOutputQueue();

	delete this->encoder;
}

//Private methods
Packet *DecoderThread::GetNextInputPacket()
{
	Packet *pPacket;

	this->inputPacketQueueLock.Lock();
	while(this->inputPacketQueue.IsEmpty())
	{
		this->inputPacketQueueSignal.Wait(this->inputPacketQueueLock);

		if(this->shutdown)
		{
			this->inputPacketQueueLock.Unlock();
			return nullptr;
		}
		if(!this->work)
		{
			this->inputPacketQueueLock.Unlock();
			return nullptr;
		}
	}
	pPacket = this->inputPacketQueue.PopFront();
	this->inputPacketQueueLock.Unlock();

	return pPacket;
}

int32 DecoderThread::ThreadMain()
{
	while(true)
	{
		//check if kill thread
		if(this->shutdown)
			break;

		//wait for work
		if(this->WaitForWork())
			continue;

		//check if we can work
		if(!this->decoder)
		{
			this->Stop();
			continue;
		}

		//lets work
		this->working = true;

		if(this->decoder->IsFrameReady())
		{
			Frame *frame = this->decoder->GetNextFrame();

			//encode as the desired playback format
			Packet *reEncodedPacket = new Packet;
			this->encoder->Encode(*frame, *reEncodedPacket);

			//this packet is ready to be played
			this->AddOutputPacket(reEncodedPacket);

			delete frame;

			continue; //go to begin
		}

		//We have no more frames for playback... decode new ones
		Packet *packet = this->GetNextInputPacket();
		if(packet)
		{
			this->decoder->Decode(*packet);
			delete packet;
		}
	}

	this->working = false;

	return EXIT_SUCCESS;
}

bool DecoderThread::WaitForWork()
{
	//wait for the instruction to work
	if(!this->work)
	{
		this->working = false;
		this->workLock.Lock();
		this->workSignal.Wait(this->workLock);
		this->workLock.Unlock();
		if(this->work)
		{
			if(this->streamIndex != Natural<uint32>::Max())
				this->decoder = this->player->GetDemuxer()->GetStream(this->streamIndex)->GetDecoder();
		}

		return true;
	}

	return false;
}

//Public methods
void DecoderThread::FlushInputQueue()
{
	this->inputPacketQueueLock.Lock();
	while(!this->inputPacketQueue.IsEmpty())
		delete this->inputPacketQueue.PopFront();
	this->inputPacketQueueLock.Unlock();
}

void DecoderThread::FlushOutputQueue()
{
	this->outputPacketQueueLock.Lock();
	while(!this->outputPacketQueue.IsEmpty())
		delete this->outputPacketQueue.PopFront();
	this->outputPacketQueueLock.Unlock();
}

Packet *DecoderThread::TryGetNextOutputPacket()
{
	Packet *packet = nullptr;

	this->outputPacketQueueLock.Lock();
	if(!this->outputPacketQueue.IsEmpty())
		packet = this->outputPacketQueue.PopFront();
	this->outputPacketQueueLock.Unlock();

	return packet;
}