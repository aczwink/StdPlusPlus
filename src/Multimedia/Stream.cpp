/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Stream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
Stream::Stream(DataType dataType)
{
	this->decoderContext = nullptr;
	this->parserContext = nullptr;
	this->encoderContext = nullptr;

	this->codingParameters.dataType = dataType;

	this->parserFlags.requiresParsing = true;
	this->parserFlags.repack = true;

	this->startTime = Unsigned<uint64>::Max();
	this->duration = Unsigned<uint64>::Max();
}

//Destructor
Stream::~Stream()
{
	delete this->parserContext;
	delete this->decoderContext;
	delete this->encoderContext;
}

//Public methods
bool Stream::AllDecodingInfoIsAvailable()
{
	if(!this->decoderContext) //we don't have a decoder
		return false;

	switch(this->codingParameters.dataType)
	{
		case DataType::Audio:
		{
			if(!this->codingParameters.audio.sampleFormat.HasValue() || this->codingParameters.audio.sampleRate == 0)
				return false;
		}
		break;
		case DataType::Video:
		{
			if((this->codingParameters.video.size.width == 0) || (this->codingParameters.video.size.height == 0))
				return false;
		}
		break;
	}

	return true;
}

bool Stream::AllInfoIsAvailable()
{
	if(!this->codingParameters.vbr.HasValue())
		return false;

	if(!this->codingParameters.bitRate)
		return false;

	return this->AllDecodingInfoIsAvailable();
}