/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/MIDI/EventStreamer.hpp>
//Namespaces
using namespace StdXX::MIDI;

//Public methods
bool EventStreamer::FindNextEvent(NextEventResult& result)
{
	int64 smallestT = Signed<int64>::Min();

	const auto& metaTrack = this->program.MetaTrack();
	if(this->metaTrackIdx < metaTrack.GetNumberOfElements())
	{
		smallestT = metaTrack[this->metaTrackIdx].timeStamp;
		result.trackType = TrackType::MetaTrack;
		result.trackIndex = this->metaTrackIdx;
	}

	for(uint32 channelNumber = 0; channelNumber < this->channelIndices.GetNumberOfElements(); channelNumber++)
	{
		uint32 channelIndex = this->channelIndices[channelNumber];
		const auto& channelTrack = this->program.GetChannelTrack(channelNumber);
		if(channelIndex < channelTrack.GetNumberOfElements())
		{
			if(channelTrack[channelIndex].timeStamp < smallestT)
			{
				smallestT = channelTrack[channelIndex].timeStamp;
				result.trackType = TrackType::ChannelTrack;
				result.channelTrackNumber = channelNumber;
				result.trackIndex = channelIndex;
			}
		}
	}

	result.deltaTime = smallestT - this->t;
	this->Update(result);
	return smallestT >= 0;
}

//Private methods
void EventStreamer::Update(const NextEventResult& nextEventResult)
{
	this->t += nextEventResult.deltaTime;

	switch(nextEventResult.trackType)
	{
		case TrackType::ChannelTrack:
			this->channelIndices[nextEventResult.channelTrackNumber]++;
			break;
		case TrackType::MetaTrack:
			this->metaTrackIdx++;
			break;
	}
}