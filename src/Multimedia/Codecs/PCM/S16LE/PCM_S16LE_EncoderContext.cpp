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
#include "PCM_S16LE_EncoderContext.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Multimedia/AudioBuffer.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Multimedia/Packet.hpp>

//Public methods
/*
void PCM_S16LE_EncoderContext::Encode(const Frame &frame)
{
	ASSERT(frame.GetType() == DataType::Audio, u8"If you see this, report to StdXX");
	const AudioFrame &audioFrame = (const AudioFrame &)frame;
	
	ASSERT(this->GetStream().GetType() == DataType::Audio, u8"If you see this, report to StdXX");
	AudioStream &audioStream = (AudioStream &)this->GetStream();

	ASSERT(audioStream.sampleFormat->sampleType == AudioSampleType::S16, u8"TODO: implement this so that it works correctly");
	
	const AudioBuffer *audioBuffer = audioFrame.GetAudioBuffer();

	//init packet info
	Packet *packet = new Packet;
	packet->Allocate(audioBuffer->GetNumberOfSamplesPerChannel() * audioStream.sampleFormat->nChannels);
	packet->pts = frame.pts;

	BufferOutputStream packetStream(packet->GetData(), packet->QuerySize());
	DataWriter packetWriter(false, packetStream);

	//write pcm data
	for(uint32 i = 0; i < audioBuffer->GetNumberOfSamplesPerChannel(); i++)
	{
		for(uint8 ch = 0; ch < audioStream.sampleFormat->nChannels; ch++)
		{
			const auto channelInfo = audioStream.sampleFormat->channels[ch];
			//get correct plane
			const byte *src = (const byte *)audioBuffer->GetPlane(channelInfo.planeIndex);
			//move to right sample
			src += audioBuffer->GetPlaneBlockSize(channelInfo.planeIndex);
			//pick sample for channel
			src += channelInfo.offset;
			
			const int16 *srcTyped = (const int16 *)src;
			packetWriter.WriteInt16(src[i]);
		}
	}
	this->AddPacket(packet);
}*/

void PCM_S16LE_EncoderContext::Flush()
{
	//this encoder always writes through
}