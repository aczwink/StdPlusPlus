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
//Main header
#include "WAVE.hpp"
//Local
#include <Std++/Streams/Readers/DataReader.hpp>
#include "../../CodingFormatIdMap.hpp"
//Namespaces
using namespace _stdxx_;

//Local functions
static CodingFormatIdMap<uint16> LoadMap()
{
	CodingFormatIdMap<uint16> twoCCMap;

	twoCCMap.Insert(1, CodingFormatId::PCM_S16LE); //WAVE_FORMAT_PCM
	twoCCMap.Insert(3, CodingFormatId::PCM_Float32LE); //WAVE_FORMAT_IEEE_FLOAT
	//twoCCMap.Insert(0x50, CodingFormatId::MP2); //TODO: could also be layer 1

	return twoCCMap;
}

//Functions
void AddMS_TwoCC_AudioCodecs(DynamicArray<CodingFormatId>& codingFormats)
{
	auto map = LoadMap();

	for(const auto& kv : map)
		codingFormats.Push(kv.value);
}

/*
uint16 MapToTwoCC(CodecId codecId)
{
	LoadMap();

	for (const auto &refKV : g_ms_audio_twoCC_map)
		if (refKV.value == codecId)
			return refKV.key;

	NOT_IMPLEMENTED_ERROR;
	return -1;
}*/

void ReadWaveHeader(InputStream& inputStream, Stream &stream, uint32 chunkSize, uint16 &refBlockAlign, uint16 &refBitsPerSample)
{
	uint16 formatTag, nChannels;
	uint32 sampleRate, avgBytesPerSecond;

	DataReader dataReader(false, inputStream);

	formatTag = dataReader.ReadUInt16();
	nChannels = dataReader.ReadUInt16();
	sampleRate = dataReader.ReadUInt32();
	avgBytesPerSecond = dataReader.ReadUInt32();
	refBlockAlign = dataReader.ReadUInt16();
	refBitsPerSample = dataReader.ReadUInt16();

	if(chunkSize >= 18) //WAVEFORMATEX
	{
		uint16 cbSize = dataReader.ReadUInt16();
		chunkSize -= 18;
		ASSERT_EQUALS(cbSize, chunkSize);
		inputStream.Skip(cbSize);
	}

	AudioSampleType sampleType;
	switch(formatTag)
	{
		case 1: //PCM
		{
			switch(refBitsPerSample)
			{
				case 16:
					sampleType = AudioSampleType::S16;
					stream.codingParameters.codingFormat = FormatRegistry::Instance().FindCodingFormatById(CodingFormatId::PCM_S16LE);
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}

			stream.codingParameters.vbr = false;
		}
		break;
		case 2: //MS ADPCM
		{
			stream.codingParameters.codingFormat = FormatRegistry::Instance().FindCodingFormatById(CodingFormatId::MS_ADPCM);
			//(stream.GetDecoder())->SetBlockAlign(refBlockAlign);
		}
		break;
		case 3: //IEEE Float
		{
			switch(refBitsPerSample)
			{
				case 32:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
					//pDecoder = (AAudioDecoder *)GetDecoder(CODEC_ID_PCM_FLOAT32LE);
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
			break;
		case 0x55: //MP3
		{
			stream.codingParameters.codingFormat = FormatRegistry::Instance().FindCodingFormatById(CodingFormatId::MP3);
		}
		break;
		case 0xFF: //MEDIASUBTYPE_RAW_AAC1
		{
			stream.codingParameters.codingFormat = FormatRegistry::Instance().FindCodingFormatById(CodingFormatId::AAC);
		}
			break;
		case 0x2000: //WAVE_FORMAT_DVM
		{
			//... AC-3 as of ffmpeg
			stream.codingParameters.codingFormat = FormatRegistry::Instance().FindCodingFormatById(CodingFormatId::AC3);
		}
			break;
		default:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	stream.codingParameters.bitRate = avgBytesPerSecond * 8;
	stream.codingParameters.audio.sampleFormat = AudioSampleFormat(nChannels, sampleType, false);
	stream.codingParameters.audio.sampleRate = sampleRate;
}