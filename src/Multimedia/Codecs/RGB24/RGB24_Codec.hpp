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
#include <Std++/Multimedia/Codec.hpp>
//Local
#include "RGB24_Encoder.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class RGB24_Codec : public Codec
{
public:
	//Methods
	Decoder *CreateDecoder(Stream &refStream) const
	{
		NOT_IMPLEMENTED_ERROR;
		//#include "CRGB24Decoder.h"
		//return new CRGB24Decoder((CVideoStream &)refStream);
		return nullptr;
	}

	Encoder *CreateEncoder(Stream &stream) const
	{
		return new RGB24_Encoder;
	}

	AParser *CreateParser() const
	{
		return nullptr;
	}

	CodecId GetId() const
	{
		return CodecId::RGB24;
	}

	String GetName() const
	{
		return u8"24-bit R8G8B8 Interleaved (RGB24)";
	}
};