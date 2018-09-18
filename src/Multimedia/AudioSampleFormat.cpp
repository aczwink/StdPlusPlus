/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/AudioSampleFormat.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
AudioSampleFormat::AudioSampleFormat(uint8 nChannels, AudioSampleType sampleType, bool planar)
	: nChannels(nChannels), sampleType(sampleType)
{
	if (planar)
	{
		this->nPlanes = this->nChannels;
		switch (this->nChannels)
		{
		case 2:
			this->channels[0].offset = 0;
			this->channels[0].planeIndex = 0;
			this->channels[1].offset = 0;
			this->channels[1].planeIndex = 1;
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}
	}
	else
	{
		this->nPlanes = 1;

		uint8 offset = 0;
		for (uint8 i = 0; i < this->nChannels; i++)
		{
			auto &ch = this->channels[i];

			ch.planeIndex = 0;
			ch.offset = offset;

			offset += this->GetSampleSize();
		}
	}

	//speaker positions
	switch (this->nChannels)
	{
	case 2: //stereo
		this->channels[0].speaker = SpeakerPosition::Front_Left;
		this->channels[1].speaker = SpeakerPosition::Front_Right;
		break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
}

//Public methods
uint8 AudioSampleFormat::ComputeBlockSize(uint8 planeIndex) const
{
	uint8 nBytesMax = 0;
	for (uint8 i = 0; i < this->nChannels; i++)
	{
		const auto &ch = this->channels[i];
		if (ch.planeIndex == planeIndex)
			nBytesMax = Math::Max(nBytesMax, uint8(ch.offset + this->GetSampleSize()));
	}

	return nBytesMax;
}

uint8 AudioSampleFormat::GetSampleSize() const
{
	switch (this->sampleType)
	{
	case AudioSampleType::S16:
		return 2;
	case AudioSampleType::Float:
		return 4;
	}

	NOT_IMPLEMENTED_ERROR;
	return 0;
}