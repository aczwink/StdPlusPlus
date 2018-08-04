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
#include "PCM_S16LE_Encoder.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/AudioBuffer.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/Packet.hpp>

//Public methods
void PCM_S16LE_Encoder::Encode(const Frame &frame)
{
	bool deleteSourceBuffer;
	uint8 ch;
	int16 *pData;
	uint32 i;
	const AbstractAudioBuffer *pAudioBuffer;
	const AudioBuffer<int16> *sourceBuffer;

	ASSERT(frame.GetType() == DataType::Audio, "If you see this, report to StdXX");

	const AudioFrame &refAudioFrame = (const AudioFrame &)frame;

	//convert if needed
	pAudioBuffer = refAudioFrame.GetAudioBuffer();
	if(pAudioBuffer->GetSampleType() == EAudioSampleType::PCM_S16LE)
	{
		sourceBuffer = (const AudioBuffer<int16> *)pAudioBuffer;
		deleteSourceBuffer = false;
	}
	else
	{
		sourceBuffer = new AudioBuffer<int16>(*pAudioBuffer);
		deleteSourceBuffer = true;
	}

	//init packet info
	Packet *packet = new Packet;
	packet->Allocate(sourceBuffer->GetSize());
	packet->pts = frame.pts;

	//write pcm data
	pData = (int16 *)packet->GetData();
	for(i = 0; i < sourceBuffer->GetNumberOfSamplesPerChannel(); i++)
	{
		for(ch = 0; ch < sourceBuffer->GetNumberOfChannels(); ch++)
		{
			*pData++ = sourceBuffer->GetChannel((Channel)ch)[i];
		}
	}
	this->AddPacket(packet);

	//clean up
	if(deleteSourceBuffer)
		delete sourceBuffer;
}

void PCM_S16LE_Encoder::Flush()
{
	//this encoder always writes through
}