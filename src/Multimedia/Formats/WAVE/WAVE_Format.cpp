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
#include "WAVE_Format.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "WAVE_Muxer.hpp"
#include "WAVE.h"

//Public methods
Demuxer *WAVE_Format::CreateDemuxer(SeekableInputStream &refInput) const
{
	return nullptr;
}

Muxer *WAVE_Format::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return new WAVE_Muxer(*this, refOutput);
}

CodecId WAVE_Format::GetDefaultCodec(DataType dataType) const
{
	NOT_IMPLEMENTED_ERROR;

	return CodecId::Unknown;
}

String WAVE_Format::GetExtension() const
{
	return u8"wav";
}

void WAVE_Format::GetFormatInfo(FormatInfo &refFormatInfo) const
{
	NOT_IMPLEMENTED_ERROR;
}

String WAVE_Format::GetName() const
{
	return u8"Waveform Audio (WAVE)";
}

FiniteSet<CodecId> WAVE_Format::GetSupportedCodecs(DataType dataType) const
{
	FiniteSet<CodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 WAVE_Format::Matches(BufferInputStream &inputStream) const
{
	if(inputStream.GetRemainingBytes() < 12)
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	DataReader reader(true, inputStream);

	if(reader.ReadUInt32() != WAVE_RIFFCHUNK_CHUNKID)
		return 0;
	inputStream.Skip(4);
	if(reader.ReadUInt32() != WAVE_RIFFCHUNK_RIFFTYPE)
		return 0;

	return 1;
}