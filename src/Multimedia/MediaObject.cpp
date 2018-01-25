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
#include <ACStdLib/Multimedia/MediaObject.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

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
	uint32 i;
	uint64 startTime, endTime;
	Stream *pStream;
	Fraction timeScale;

	//compute overall time scale
	if(this->timeScale == Fraction()) //but only if demuxer or muxer does not set a specific one
	{
		for(uint32 i = 0; i < this->GetNumberOfStreams(); i++)
		{
			timeScale = this->GetStream(i)->timeScale;
			if(timeScale == Fraction())
				continue; //no time scale for this stream

			timeScale = timeScale.Reduce();
			ASSERT(timeScale.numerator == 1, "If you see this, report to ACStdLib"); //TODO: figure out what happens if numerator is not 1
			this->timeScale.denominator = ComputeLeastCommonMultiple(this->timeScale.denominator, timeScale.denominator);
		}
		this->timeScale.numerator = 1; //TODO: figure out what happens if numerator of stream is not 1
	}

	//compute start time
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		pStream = this->GetStream(i);

		if(pStream->timeScale == Fraction())
			continue; //no time scale for this stream

		if(pStream->startTime != Natural<uint64>::Max())
		{
			startTime = pStream->startTime / this->timeScale * pStream->timeScale; //keep order because of integer division
			if(startTime < this->startTime)
				this->startTime = startTime;
		}
	}

	//compute longest duration
	for(i = 0; i < this->GetNumberOfStreams(); i++)
	{
		pStream = this->GetStream(i);

		if(pStream->timeScale == Fraction())
			continue; //no time scale for this stream
		if(pStream->startTime == Natural<uint64>::Max() || pStream->duration == Natural<uint64>::Max())
			continue; //no start time or duration

		endTime = (pStream->startTime + pStream->duration) / this->timeScale * pStream->timeScale;
		if(endTime - this->startTime > this->duration || this->duration == Natural<uint64>::Max())
			this->duration = endTime - this->startTime;
	}
}