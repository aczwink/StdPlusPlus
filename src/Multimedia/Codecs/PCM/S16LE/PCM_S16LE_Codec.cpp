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
#include "PCM_S16LE_Codec.hpp"
//Local
#include "PCM_S16LE_Encoder.hpp"

//Public methods
Decoder *PCM_S16LE_Codec::CreateDecoder(Stream &refStream) const
{
	return nullptr;
}

Encoder *PCM_S16LE_Codec::CreateEncoder(Stream &stream) const
{
	return new PCM_S16LE_Encoder;
}

ParserContext *PCM_S16LE_Codec::CreateParser() const
{
	return nullptr;
}
/*
CodecId PCM_S16LE_Codec::GetId() const
{
	return CodecId::PCM_S16LE;
}
*/
/*
String PCM_S16LE_Codec::GetName() const
{
	return u8"PCM signed 16-bit little-endian interleaved channels";
}*/