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
#include "PCM_S16LE_Encoder.hpp"
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Multimedia/AudioBuffer.hpp>
#include <ACStdLib/Multimedia/AudioFrame.hpp>
#include <ACStdLib/Multimedia/Packet.hpp>

//Public methods
void PCM_S16LE_Encoder::Encode(const Frame &frame, Packet &packet) const
{
	bool deleteSourceBuffer;
	uint8 ch;
	int16 *pData;
	uint32 i;
	const AbstractAudioBuffer *pAudioBuffer;
	const AudioBuffer<int16> *sourceBuffer;

	ASSERT(frame.GetType() == DataType::Audio);

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
	packet.Allocate(sourceBuffer->GetSize());
	packet.pts = frame.pts;

	//write pcm data
	pData = (int16 *)packet.GetData();
	for(i = 0; i < sourceBuffer->GetNumberOfSamplesPerChannel(); i++)
	{
		for(ch = 0; ch < sourceBuffer->GetNumberOfChannels(); ch++)
		{
			*pData++ = sourceBuffer->GetChannel((Channel)ch)[i];
		}
	}

	//clean up
	if(deleteSourceBuffer)
		delete sourceBuffer;
}