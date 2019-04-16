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
//Class Header
#include "MatroskaVideo.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "../BMP/BMP.hpp"
#include "../WAVE/WAVE.h"
#include "EBML.hpp"
#include "MatroskaDemuxer.hpp"
#include "MatroskaMuxer.hpp"

//Public methods
Demuxer *MatroskaVideo::CreateDemuxer(SeekableInputStream &refInput) const
{
	return new MatroskaDemuxer(*this, refInput);
}

Muxer *MatroskaVideo::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return new MatroskaMuxer(*this, refOutput);
}

/*CodecId MatroskaVideo::GetDefaultCodec(DataType dataType) const
{
	NOT_IMPLEMENTED_ERROR;

	return CodecId::Unknown;
}*/

String MatroskaVideo::GetExtension() const
{
	return u8"mkv";
}

void MatroskaVideo::GetFormatInfo(FormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = false;
}

String MatroskaVideo::GetName() const
{
	return u8"Matroska Video";
}

/*BinaryTreeSet<CodecId> MatroskaVideo::GetSupportedCodecs(DataType dataType) const
{
	BinaryTreeSet<CodecId> result;

	switch(dataType)
	{
		case DataType::Audio:
		{
			AddMS_TwoCC_AudioCodecs(result);
		}
			break;
		case DataType::Subtitle:
		{
			NOT_IMPLEMENTED_ERROR;
		}
			break;
		case DataType::Video:
		{
			AddMS_FourCC_VideoCodecs(result);

			result.Insert(CodecId::MPEG2Video);
		}
			break;
	}

	return result;
}*/

float32 MatroskaVideo::Matches(BufferInputStream &buffer) const
{
	EBML::Header header;
	if(!EBML::ParseHeader(header, buffer))
	{
		if(buffer.IsAtEnd())
			return FORMAT_MATCH_BUFFER_TOO_SMALL;

		return 0;
	}

	if(header.version != 1)
		return 0;
	if(header.docType != u8"matroska")
		return 0;
	if(header.docTypeVersion > 2)
		return 0;

	return 1;
}