/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "libavcodec_Encoder.hpp"
//Local
#include <Std++/Containers/Strings/String.hpp>
#include "libavcodec_EncoderContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Class header
EncoderContext *_stdxx_::libavcodec_Encoder::CreateContext(Stream &stream) const
{
	return new libavcodec_EncoderContext(stream, this->codec);
}

CodingFormatId _stdxx_::libavcodec_Encoder::GetCodingFormatId() const
{
	return this->codingFormatId;
}

String _stdxx_::libavcodec_Encoder::GetName() const
{
	return u8"'" + String(this->codec->name) + u8"' (via libavcodec)";
}