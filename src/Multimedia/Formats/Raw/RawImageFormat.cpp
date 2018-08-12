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
#include "RawImageFormat.hpp"
//Local
#include "RawImageDemuxer.hpp"
#include "RawImageMuxer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
Demuxer *RawImageFormat::CreateDemuxer(SeekableInputStream &refInput) const
{
	return new RawImageDemuxer(*this, refInput, this->codingFormatId);
}

Muxer *RawImageFormat::CreateMuxer(ASeekableOutputStream &outputStream) const
{
	return new RawImageMuxer(*this, outputStream);
}

/*CodecId RawImageFormat::GetDefaultCodec(DataType dataType) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	if(dataType == DataType::Video)
		return CodecId::Unknown;

	return CodecId::Unknown;
}*/

void RawImageFormat::GetFormatInfo(FormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = false;
}

String RawImageFormat::GetName() const
{
	return u8"Raw image stream";
}
