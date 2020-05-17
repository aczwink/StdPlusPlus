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
#include <Std++/Multimedia/MediaObject.hpp>
//Local
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Destructor
MediaObject::~MediaObject()
{
	for(const Stream *const& refpStream : this->streams)
	{
		delete refpStream;
	}
}

//Protected methods
void MediaObject::UpdateTimingInfo()
{
	//compute overall time scale
	if(this->timeScale == Math::Rational<uint64>()) //but only if demuxer or muxer does not set a specific one
	{
		for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
		{
			Math::Rational timeScale = this->GetStream(i)->timeScale;
			if(timeScale == Math::Rational<uint64>())
				continue; //no time scale for this stream

			timeScale = timeScale.Reduce();
			this->timeScale.denominator = Math::ComputeLeastCommonMultiple(this->timeScale.denominator, timeScale.denominator);
		}

		this->timeScale.numerator = 1;
	}

	//compute start time
	Stream *pStream;
	for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		pStream = this->GetStream(i);

		if(pStream->timeScale == Math::Rational<uint64>())
			continue; //no time scale for this stream

		if(pStream->startTime != Unsigned<uint64>::Max())
		{
			uint64 startTime = pStream->timeScale.Rescale(pStream->startTime, this->timeScale);
			if(startTime < this->startTime)
				this->startTime = startTime;
		}
	}

	//compute longest duration
	for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
	{
		pStream = this->GetStream(i);

		if(pStream->timeScale == Math::Rational<uint64>())
			continue; //no time scale for this stream
		if(pStream->startTime == Unsigned<uint64>::Max() || pStream->duration == Unsigned<uint64>::Max())
			continue; //no start time or duration

		uint64 endTime = pStream->timeScale.Rescale(pStream->startTime + pStream->duration, this->timeScale);
		if(endTime - this->startTime > this->duration || this->duration == Unsigned<uint64>::Max())
			this->duration = endTime - this->startTime;
	}
}