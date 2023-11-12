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
#include <Std++/Multimedia/FormatRegistry.hpp>
//Local
#include <Std++/Multimedia/Decoder.hpp>
//Audio coding formats
#include "Codecs/MPEG/AAC_CodingFormat.hpp"
#include "Codecs/MPEG/MP3/MP3_CodingFormat.hpp"
#include "Codecs/Xiph/Vorbis_CodingFormat.hpp"
#include "Codecs/AC3_CodingFormat.hpp"
#include "Codecs/PCM/Float32LE/PCM_Float32LE_CodingFormat.hpp"
#include "Codecs/PCM/U8/PCM_U8_CodingFormat.hpp"
#include "Codecs/PCM/S16/PCM_S16BE_CodingFormat.hpp"
#include "Codecs/PCM/S8/PCM_S8_CodingFormat.hpp"

//Subtitle coding formats
#include "Codecs/Raw_Subtitles/UTF8PlainText_CodingFormat.hpp"

//Video coding formats
#include "Codecs/Images/PNG_CodingFormat.hpp"
#include "Codecs/MPEG/H264_CodingFormat.hpp"
#include "Codecs/MPEG/MPEG4_Part2_CodingFormat.hpp"
#include "Codecs/MPEG/MS_MPEG4_Part2_V2_CodingFormat.hpp"
#include "Codecs/RawVideo/RawVideo_CodingFormat.hpp"
#include "Codecs/S3TC/S3TC_DXT1_CodingFormat.hpp"
#include "Codecs/Xiph/Theora_CodingFormat.hpp"

//Container formats
#include "Formats/BMP/BMP_Format.hpp"
#include "Formats/DDS/DDS_Format.hpp"
#include "Formats/Matroska/MatroskaVideo.hpp"
#include "Formats/Raw/RawContainerImages.hpp"
#include "Formats/WAVE/WAVE_Format.hpp"
#include "Codecs/PCM/S16/PCM_S16LE_CodingFormat.hpp"

//Decoders
#include "Codecs/PCM/S16/PCM_S16_Decoder.hpp"
#include "Codecs/S3TC/S3TC_DXT1_Decoder.hpp"
#include "Codecs/RawVideo/RawVideoDecoder.hpp"

//Encoders
#include "Codecs/PCM/S16/PCM_S16_Encoder.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Private methods
void FormatRegistry::RegisterCodingFormats()
{
	//Audio coding formats
	this->Register(new AAC_CodingFormat);
	this->Register(new AC3_CodingFormat);
	this->Register(new MP3_CodingFormat);
	this->Register(new PCM_Float32LE_CodingFormat);
	this->Register(new PCM_S16BE_CodingFormat);
	this->Register(new PCM_S16LE_CodingFormat);
	this->Register(new PCM_S8_CodingFormat);
	this->Register(new PCM_U8_CodingFormat);
	this->Register(new Vorbis_CodingFormat);


	//Subtitle coding formats
	this->Register(new UTF8PlainText_CodingFormat);


	//Video coding formats
	this->Register(new H264_CodingFormat);
	this->Register(new MPEG4_Part2_CodingFormat);
	this->Register(new MS_MPEG4_Part2_V2_CodingFormat);
	this->Register(new PNG_CodingFormat);
	this->Register(new RawVideo_CodingFormat);
	this->Register(new S3TC_DXT1_CodingFormat);
	this->Register(new Theora_CodingFormat);
}

void FormatRegistry::RegisterContainerFormats()
{
	this->Register(new BMP_Format);
	this->Register(new DDS_Format);
	this->Register(new MatroskaVideo);
	this->Register(new PNG_Format);
	this->Register(new WAVE_Format);
}

void FormatRegistry::RegisterDecoders()
{
	this->Register(new PCM_S16_Decoder(true), 0.0f);
	this->Register(new PCM_S16_Decoder(false), 0.0f);
	this->Register(new RawVideoDecoder, 0.0f);
	this->Register(new S3TC_DXT1_Decoder, 0.0f);
}

void FormatRegistry::RegisterEncoders()
{
	this->Register(new PCM_S16_Encoder(true), 0.0f);
	this->Register(new PCM_S16_Encoder(false), 0.0f);
}