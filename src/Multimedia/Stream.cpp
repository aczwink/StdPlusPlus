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
#include <ACStdLib/Multimedia/Stream.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Constructor
Stream::Stream()
{
	this->pCodec = nullptr;
	this->pDecoder = nullptr;
	this->pParser = nullptr;
	this->pEncoder = nullptr;

	this->startTime = UINT64_MAX;
	this->duration = UINT64_MAX;
	this->vbr = false;
	this->bitRate = 0;
}

//Destructor
Stream::~Stream()
{
	if(this->pParser)
		delete this->pParser;
	if(this->pDecoder)
		delete this->pDecoder;
	if(this->pEncoder)
		delete this->pEncoder;
}

//Public methods
bool Stream::AllInfoIsAvailable()
{
	if(!this->bitRate)
		return false;

	if(!this->GetDecoder()) //we don't have a decoder
		return false;

	return this->AllDecoderInfoIsAvailable();
}

Decoder *Stream::GetDecoder()
{
	if(this->pDecoder == NULL)
	{
		//we have no decoder... try to instantiate one
		if(this->pCodec)
		{
			//we have a codec... create instance
			this->pDecoder = this->pCodec->CreateDecoder(*this);
			return this->pDecoder;
		}

		return NULL;
	}

	return this->pDecoder;
}

Encoder *Stream::GetEncoder()
{
	if(this->pEncoder == NULL)
	{
		//we have no encoder... try to instantiate one
		if(this->pCodec)
		{
			//we have a codec... create instance
			this->pEncoder = this->pCodec->CreateEncoder();
			return this->pEncoder;
		}

		return NULL;
	}

	return this->pEncoder;
}

AParser *Stream::GetParser()
{
	if(this->pParser == nullptr)
	{
		//try to instantiate
		if(this->pCodec)
			this->pParser = this->pCodec->CreateParser();
	}

	return this->pParser;
}