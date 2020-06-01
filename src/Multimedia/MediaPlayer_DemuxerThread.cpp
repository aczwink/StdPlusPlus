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
#include <Std++/Multimedia/MediaPlayer.hpp>
//Local
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
DemuxerThread::DemuxerThread(Demuxer *demuxer)
{
	this->demuxer = demuxer;
	this->shutdown = false;
	this->work = false;
	this->working = false;
	this->audioStreamIndex = Unsigned<uint32>::Max();
	this->videoStreamIndex = Unsigned<uint32>::Max();
}

//Private methods
void DemuxerThread::Run()
{
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
		UniquePointer<IPacket> packet = demuxer->ReadFrame();
		if(packet.IsNull())
		{
			this->work = false; //no more work
			continue;
		}

		//check if we want that packet
		if(packet->GetStreamIndex() == this->audioStreamIndex)
		{
			this->audioDecodeThread->AddInputPacket(Move(packet));
		}
		else if(packet->GetStreamIndex() == this->videoStreamIndex)
		{
			this->videoDecodeThread->AddInputPacket(Move(packet));
		}
		//else not of interest
	}

	this->working = false;
}