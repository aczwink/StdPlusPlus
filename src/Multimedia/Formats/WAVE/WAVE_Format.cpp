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
#include "WAVE_Format.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "WAVE_Muxer.hpp"
#include "WAVE.hpp"
#include "WAVE_Demuxer.hpp"

//Public methods
Demuxer *WAVE_Format::CreateDemuxer(SeekableInputStream& inputStream) const
{
	return new WAVE_Demuxer(*this, inputStream);
}

Muxer *WAVE_Format::CreateMuxer(SeekableOutputStream &refOutput) const
{
	return new WAVE_Muxer(*this, refOutput);
}

String WAVE_Format::GetExtension() const
{
	return u8"wav";
}

void WAVE_Format::GetFormatInfo(FormatInfo& formatInfo) const
{
	formatInfo.supportsByteSeeking = true;
}

String WAVE_Format::GetName() const
{
	return u8"Waveform Audio (WAVE)";
}

DynamicArray<const CodingFormat *> WAVE_Format::GetSupportedCodingFormats(DataType dataType) const
{
	DynamicArray<const CodingFormat *> codingFormats;

	if(dataType != DataType::Audio)
		return  codingFormats;

	DynamicArray<CodingFormatId> codingFormatIds;
	AddMS_TwoCC_AudioCodecs(codingFormatIds);

	for(const CodingFormatId id : codingFormatIds)
		codingFormats.Push(FormatRegistry::GetCodingFormatById(id));

	return codingFormats;
}

float32 WAVE_Format::Probe(BufferInputStream &inputStream) const
{
	if(inputStream.QueryRemainingBytes() < 12)
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	DataReader reader(false, inputStream);

	if(reader.ReadUInt32() != WAVE_RIFFCHUNK_CHUNKID)
		return 0;
	inputStream.Skip(4);
	if(reader.ReadUInt32() != WAVE_RIFFCHUNK_RIFFTYPE)
		return 0;

	return 1;
}
