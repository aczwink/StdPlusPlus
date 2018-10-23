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
/*
//Video codecs
#include "Codecs/RGB24/RGB24_Codec.hpp"
#include "Codecs/Images/PNG_CodingFormat.hpp"

void MultimediaRegisterCodecsAndFormats()
{
	//Codecs
	//Audio codecs
	Codec::Register(new PCM_S16LE_Codec);

	//Video codecs
	Codec::Register(new H264_Codec);
	Codec::Register(new RGB24_Codec);
}
*/

//Audio coding formats
#include "Codecs/MPEG/AAC_CodingFormat.hpp"
#include "Codecs/MPEG/MP3/MP3_CodingFormat.hpp"
#include "Codecs/PCM/S16LE/PCM_S16LE_CodingFormat.hpp"

//Video coding formats
#include "Codecs/Images/PNG_CodingFormat.hpp"
#include "Codecs/MPEG/H264_CodingFormat.hpp"
#include "Codecs/MPEG/MS_MPEG4_Part2_V2_CodingFormat.hpp"
#include "Codecs/RawVideo/RawVideo_CodingFormat.hpp"

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


	//Video coding formats
	CodingFormat::Register(new H264_CodingFormat);
	CodingFormat::Register(new MS_MPEG4_Part2_V2_CodingFormat);
	CodingFormat::Register(new PNG_CodingFormat);
	CodingFormat::Register(new RawVideo_CodingFormat);


	//Container formats
	Format::Register(new BMP_Format);
	Format::Register(new MatroskaVideo);
	Format::Register(new PNG_Format);
	Format::Register(new WAVE_Format);
}