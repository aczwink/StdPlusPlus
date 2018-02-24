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
#include "BMP_Format.hpp"
//Local
#include "BMP.h"
//#include "CBMPDemuxer.h"
#include "BMP_Muxer.hpp"

//Public methods
Demuxer *BMP_Format::CreateDemuxer(SeekableInputStream &refInput) const
{
	//return new CBMPDemuxer(*this, refInput);
	NOT_IMPLEMENTED_ERROR;
	return nullptr;
}

Muxer *BMP_Format::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return new BMP_Muxer(*this, refOutput);
}

CodecId BMP_Format::GetDefaultCodec(DataType dataType) const
{
	if(dataType == DataType::Video)
		return CodecId::BGR24;

	return CodecId::Unknown;
}

OldString BMP_Format::GetExtension() const
{
	return "bmp";
}

void BMP_Format::GetFormatInfo(FormatInfo &refFormatInfo) const
{
	NOT_IMPLEMENTED_ERROR;
}

OldString BMP_Format::GetName() const
{
	return "BMP Bitmap image";
}

FiniteSet<CodecId> BMP_Format::GetSupportedCodecs(DataType dataType) const
{
	FiniteSet<CodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 BMP_Format::Matches(BufferInputStream &refBuffer) const
{
	byte readHeader[BMP_HEADER_TYPE_SIZE];

	refBuffer.ReadBytes(readHeader, sizeof(readHeader));
	if(MemCmp(BMP_HEADER_TYPE, readHeader, sizeof(readHeader)) == 0)
		return 1;

	if(refBuffer.IsAtEnd())
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	return 0;
}