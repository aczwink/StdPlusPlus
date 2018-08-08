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
#include <Std++/Multimedia/Stream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
Stream::Stream()
{
	this->pCodec = nullptr;
	this->decoderContext = nullptr;
	this->parserContext = nullptr;
	this->pEncoder = nullptr;

	this->startTime = Natural<uint64>::Max();
	this->duration = Natural<uint64>::Max();
	this->vbr = false;
	this->bitRate = 0;
}

//Destructor
Stream::~Stream()
{
	delete this->parserContext;
	delete this->decoderContext;
	if(this->pEncoder)
		delete this->pEncoder;
}

//Public methods
bool Stream::AllInfoIsAvailable()
{
	if(!this->bitRate)
		return false;

	if(!this->decoderContext) //we don't have a decoder
		return false;

	return this->AllDecoderInfoIsAvailable();
}

Encoder *Stream::GetEncoder()
{
	if(this->pEncoder == NULL)
	{
		//we have no encoder... try to instantiate one
		if(this->pCodec)
		{
			//we have a codec... create instance
			this->pEncoder = this->pCodec->CreateEncoder(*this);
			return this->pEncoder;
		}

		return NULL;
	}

	return this->pEncoder;
}