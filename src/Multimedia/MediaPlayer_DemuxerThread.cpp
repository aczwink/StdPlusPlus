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
//Local
#include <Std++/Natural.hpp>
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;

//Constructor
DemuxerThread::DemuxerThread(MediaPlayer *player)
{
	this->player = player;
	this->shutdown = false;
	this->work = false;
	this->working = false;
	this->audioStreamIndex = Natural<uint32>::Max();
	this->videoStreamIndex = Natural<uint32>::Max();
}

//Private methods
int32 DemuxerThread::ThreadMain()
{
	this->demuxer = this->player->GetDemuxer();

	while(true)
	{
		if(this->shutdown)
			break;

		//wait for the instruction to work
		if(!this->work)
		{
			this->working = false;
			this->workLock.Lock();
			this->workSignal.Wait(this->workLock);
			this->workLock.Unlock();
			continue;
		}
		this->working = true;

		//read a packet
		Packet *packet = new Packet;
		if(!demuxer->ReadFrame(*packet))
		{
			delete packet;
			this->work = false; //no more work
			continue;
		}

		//check if we want that packet
		if(packet->streamIndex == this->audioStreamIndex)
		{
			this->audioDecodeThread->AddInputPacket(packet);
		}
		else if(packet->streamIndex == this->videoStreamIndex)
		{
			this->videoDecodeThread->AddInputPacket(packet);
		}
		else
		{
			//not of interest
			delete packet;
		}
	}

	this->working = false;

	return EXIT_SUCCESS;
}