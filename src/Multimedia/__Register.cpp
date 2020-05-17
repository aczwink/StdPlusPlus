/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
//Audio coding formats
#include "Codecs/MPEG/AAC_CodingFormat.hpp"
#include "Codecs/MPEG/MP3/MP3_CodingFormat.hpp"
#include "Codecs/PCM/S16LE/PCM_S16LE_CodingFormat.hpp"
#include "Codecs/Xiph/Vorbis_CodingFormat.hpp"

//Subtitle coding formats
#include "Codecs/Raw_Subtitles/UTF8PlainText_CodingFormat.hpp"

//Video coding formats
#include "Codecs/Images/PNG_CodingFormat.hpp"
#include "Codecs/MPEG/H264_CodingFormat.hpp"
#include "Codecs/MPEG/MPEG4_Part2_CodingFormat.hpp"
#include "Codecs/MPEG/MS_MPEG4_Part2_V2_CodingFormat.hpp"
#include "Codecs/RawVideo/RawVideo_CodingFormat.hpp"
#include "Codecs/Xiph/Theora_CodingFormat.hpp"

//Container formats
#include "Formats/BMP/BMP_Format.hpp"
#include "Formats/Matroska/MatroskaVideo.hpp"
#include "Formats/Raw/RawContainerImages.hpp"
#include "Formats/WAVE/WAVE_Format.hpp"
//Namespaces
using namespace _stdxx_;

void MultimediaRegisterCodecsAndFormats()
{
	//Audio coding formats
	CodingFormat::Register(new AAC_CodingFormat);
	CodingFormat::Register(new MP3_CodingFormat);
	CodingFormat::Register(new PCM_S16LE_CodingFormat);
	CodingFormat::Register(new Vorbis_CodingFormat);


	//Subtitle coding formats
	CodingFormat::Register(new UTF8PlainText_CodingFormat);


	//Video coding formats
	CodingFormat::Register(new H264_CodingFormat);
	CodingFormat::Register(new MPEG4_Part2_CodingFormat);
	CodingFormat::Register(new MS_MPEG4_Part2_V2_CodingFormat);
	CodingFormat::Register(new PNG_CodingFormat);
	CodingFormat::Register(new RawVideo_CodingFormat);
	CodingFormat::Register(new Theora_CodingFormat);


	//Container formats
	Format::Register(new BMP_Format);
	Format::Register(new MatroskaVideo);
	Format::Register(new PNG_Format);
	Format::Register(new WAVE_Format);
}